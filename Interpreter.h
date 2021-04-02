#pragma once

#include <string>
#include "Lox.h"
#include "Expr.h"
#include "Environment.h"

namespace Interpreter
{
    Environment environment;

    Object evaluate(Expr *expr);

    void runtimeError(Token oper, std::string message)
    {
        Lox::runtimeError(oper, message);
    }

    bool isTruthy(Object value)
    {
        switch (value.type)
        {
            case TYPE_NIL:
                return false;
            case TYPE_BOOL:
                return value.boolLiteral;
            default:
                return true;
        }
    }

    bool isEqual(Object a, Object b)
    {
        if (a.type != b.type)
        {
            return false;
        }

        switch (a.type)
        {
            case TYPE_NIL: return true;
            case TYPE_BOOL: return a.boolLiteral == b.boolLiteral;
            case TYPE_NUMBER: return a.numLiteral == b.numLiteral;
            case TYPE_STRING: return a.strLiteral == b.strLiteral;
        }
    }

    bool checkNumberOperand(Token oper, Object value)
    {
        if (value.type == TYPE_NUMBER)
        {
            return true;
        }
        else
        {
            runtimeError(oper, "Operand must be a number.");
            return false;
        }
    }

    bool checkNumberOperands(Token oper, Object a, Object b)
    {
        if (a.type == TYPE_NUMBER && b.type == TYPE_NUMBER)
        {
            return true;
        }
        else
        {
            runtimeError(oper, "Operands must be numbers.");
            return false;
        }
    }

    Object visitTernary(Ternary *expr)
    {
        Object cond = evaluate(expr->first);

        if (isTruthy(cond))
        {
            return evaluate(expr->second);
        }
        else
        {
            return evaluate(expr->third);
        }
    }
    
    Object visitBinary(Binary *expr)
    {
        Object left = evaluate(expr->left);
        Object right = evaluate(expr->right);

        switch (expr->oper->type)
        {
            case MINUS:
            {
                checkNumberOperands(*expr->oper, left, right);
                Object value(left.numLiteral - right.numLiteral);
                return value;
            }

            case STAR:
            {
                checkNumberOperands(*expr->oper, left, right);
                Object value(left.numLiteral * right.numLiteral);
                return value;
            }

            case SLASH:
            {
                checkNumberOperands(*expr->oper, left, right);
                if (right.numLiteral == 0)
                {
                    runtimeError(*expr->oper, "Divide by zero.");
                }
                Object value(left.numLiteral / right.numLiteral);
                return value;
            }

            case PLUS:
            {
                if (left.type == TYPE_NUMBER && right.type == TYPE_NUMBER)
                {
                    Object value(left.numLiteral + right.numLiteral);
                    return value;
                }
                else if (left.type == TYPE_STRING && right.type == TYPE_STRING)
                {
                    Object value(left.strLiteral + right.strLiteral);
                    return value;
                }
                else if (left.type == TYPE_STRING)
                {
                    Object value(left.strLiteral + right.str());
                    return value;
                }
                else if (right.type == TYPE_STRING)
                {
                    Object value(left.str() + right.strLiteral);
                    return value;
                }
                else
                {
                    runtimeError(*expr->oper, "Operands must be both numbers or at least one should be a string.");
                }
            }

            case EQUAL_EQUAL:
            {
                Object value(isEqual(left, right));
                return value;
            }

            case BANG_EQUAL:
            {
                Object value(!isEqual(left, right));
                return value;
            }

            case LESS:
            {
                if (left.type == TYPE_NUMBER && right.type == TYPE_NUMBER)
                {
                    Object value(left.numLiteral < right.numLiteral);
                    return value;
                }
                else if (left.type == TYPE_STRING && right.type == TYPE_STRING)
                {
                    Object value(left.strLiteral < right.strLiteral);
                    return value;
                }
                else
                {
                    runtimeError(*expr->oper, "Operands must be both strings or both numbers.");
                }
            }

            case GREATER:
            {
                if (left.type == TYPE_NUMBER && right.type == TYPE_NUMBER)
                {
                    Object value(left.numLiteral > right.numLiteral);
                    return value;
                }
                else if (left.type == TYPE_STRING && right.type == TYPE_STRING)
                {
                    Object value(left.strLiteral > right.strLiteral);
                    return value;
                }
                else
                {
                    runtimeError(*expr->oper, "Operands must be both strings or both numbers.");
                }
            }

            case LESS_EQUAL:
            {
                if (left.type == TYPE_NUMBER && right.type == TYPE_NUMBER)
                {
                    Object value(left.numLiteral <= right.numLiteral);
                    return value;
                }
                else if (left.type == TYPE_STRING && right.type == TYPE_STRING)
                {
                    Object value(left.strLiteral <= right.strLiteral);
                    return value;
                }
                else
                {
                    runtimeError(*expr->oper, "Operands must be both strings or both numbers.");
                }
            }

            case GREATER_EQUAL:
            {
                if (left.type == TYPE_NUMBER && right.type == TYPE_NUMBER)
                {
                    Object value(left.numLiteral >= right.numLiteral);
                    return value;
                }
                else if (left.type == TYPE_STRING && right.type == TYPE_STRING)
                {
                    Object value(left.strLiteral >= right.strLiteral);
                    return value;
                }
                else
                {
                    runtimeError(*expr->oper, "Operands must be both strings or both numbers.");
                }
            }

            default:
                runtimeError(*expr->oper, "Invalid binary operator.");
        }

        return nullptr;
    }

    Object visitGrouping(Grouping *expr)
    {
        Object value = evaluate(expr->expression);
        return value;
    }

    Object visitLiteral(Literal *expr)
    {
        return *expr->value;
    }

    Object visitUnary(Unary *expr)
    {
        Object right = evaluate(expr->right);

        switch (expr->oper->type)
        {
            case MINUS:
            {
                checkNumberOperand(*expr->oper, right);
                Object value(-right.numLiteral);
                return value;
            }

            case BANG:
            {
                Object value(!isTruthy(right));
                return value;
            }

            default:
                runtimeError(*expr->oper, "Invalid unary operator.");
        }

    }

    Object visitVariable(Variable *expr)
    {
        return environment.get(expr->name);
    }

    Object evaluate(Expr *expr)
    {
        if (expr)
        {
            switch (expr->type)
            {
                case ExprType_Ternary: return visitTernary((Ternary *)expr);
                case ExprType_Binary: return visitBinary((Binary *)expr);
                case ExprType_Grouping: return visitGrouping((Grouping *)expr);
                case ExprType_Literal: return visitLiteral((Literal *)expr);
                case ExprType_Unary: return visitUnary((Unary *)expr);
                case ExprType_Variable: return visitVariable((Variable *)expr);
            }
        }
        Lox::error(EOF_TOKEN, "Invalid expression type.");
        Object nil;
        return nil;
    }

    void visitPrint(Print *stmt)
    {
        Object value = evaluate(stmt->expression);
        if (!Lox::hadRuntimeError)
        {
            std::cout << value.str() << std::endl;
        }
    }

    void visitExpression(Expression *stmt)
    {
        evaluate(stmt->expression);
    }

    void visitVar(Var *stmt)
    {
        Object value;
        if (stmt->initializer)
        {
            value = evaluate(stmt->initializer);
        }
        environment.define(stmt->name->lexeme, value);
    }

    void execute(Stmt *stmt)
    {
        if (stmt)
        {
            switch (stmt->type)
            {
                case StmtType_Print: visitPrint((Print *)stmt); break;
                case StmtType_Expression: visitExpression((Expression *)stmt); break;
                case StmtType_Var: visitVar((Var *)stmt); break;
                default:
                    Lox::error(EOF_TOKEN, "Invalid statement type.");
            }
        }        
    }

    void interpret(std::vector<Stmt *> statements)
    {
        for (auto &statement : statements)
        {
            if (statement)
            {
                execute(statement);
            }
            if (Lox::hadRuntimeError)
            {
                Lox::hadRuntimeError = false;
                break;
            }
            else if (Lox::hadError)
            {
                break;
            }
        }
    }
};