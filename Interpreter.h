#pragma once

#include <iostream>
#include <chrono>
#include <string>
#include "Lox.h"
#include "Expr.h"
#include "Stmt.h"
#include "Environment.h"
#include "LoxCallable.h"
#include "LoxFunction.h"

class Interpreter;

class ClockFunction : public LoxCallable
{
    virtual int arity() override
    {
        return 0;
    }

    virtual Object call(Interpreter *interpreter, std::vector<Object>) override
    {
        auto millisec_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        Object value((double)millisec_since_epoch / 1000.0);
        return value;
    }

    virtual std::string str() override
    {
        return "<native fn>";
    }
};

class Interpreter
{
    static Environment *environment;
    bool repl = false;
    bool breakSet = false;
    bool returnSet = false;
    Object returnValue;

public:
    static Environment *globals;

    Interpreter(bool repl)
    {
        this->repl = repl;
        if (!environment)
        {
            globals = new Environment();
            environment = globals;
            Object clock(new ClockFunction());
            environment->define("clock", clock);
        }
    }

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

        return false;
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

    Object visitAssign(Assign *expr)
    {
        Object value = evaluate(expr->value);
        environment->assign(expr->name, value);
        return value;
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

    Object visitLogical(Logical *expr)
    {
        Object left = evaluate(expr->left);

        if (expr->oper->type == AND)
        {
            if (isTruthy(left))
            {
                return evaluate(expr->right);
            }
        }
        else
        {
            if (!isTruthy(left))
            {
                return evaluate(expr->right);
            }
        }

        return left;
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

        return right;
    }

    Object visitVariable(Variable *expr)
    {
        return environment->get(expr->name);
    }

    Object visitCall(Call *stmt)
    {
        Object callee = evaluate(stmt->callee);

        std::vector<Object> arguments;
        for (auto &argument : stmt->arguments->list)
        {
            arguments.push_back(evaluate(argument));
        }

        if (callee.type != TYPE_FUNCTION)
        {
            runtimeError(*stmt->paren, "Can only call functions and methods.");
        }

        if (callee.function->arity() != arguments.size())
        {
            std::stringstream ss;
            ss << "Expected " << callee.function->arity() << " arguments but got " << arguments.size() << ".";
            runtimeError(*stmt->paren, ss.str());
            return callee;
        }
        else
        {
            return callee.function->call(this, arguments);
        }
    }

    Object evaluate(Expr *expr)
    {
        if (expr)
        {
            switch (expr->type)
            {
                case ExprType_Assign: return visitAssign((Assign *)expr);
                case ExprType_Ternary: return visitTernary((Ternary *)expr);
                case ExprType_Binary: return visitBinary((Binary *)expr);
                case ExprType_Logical: return visitLogical((Logical *)expr);
                case ExprType_Grouping: return visitGrouping((Grouping *)expr);
                case ExprType_Literal: return visitLiteral((Literal *)expr);
                case ExprType_Unary: return visitUnary((Unary *)expr);
                case ExprType_Variable: return visitVariable((Variable *)expr);
                case ExprType_Call: return visitCall((Call *)expr);
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
        Object value = evaluate(stmt->expression);
        if (repl && !Lox::hadRuntimeError)
        {
            std::cout << value.str() << std::endl;
        }
    }

    void visitVar(Var *stmt)
    {
        Object value;
        if (stmt->initializer)
        {
            value = evaluate(stmt->initializer);
        }
        else
        {
            value.type = TYPE_UNINITIALIZED;
        }
        environment->define(stmt->name, value);
    }

    Object executeBlock(Block *stmt, Environment *execEnvironment)
    {
        Environment *savedEnvironment = environment;
        environment = execEnvironment;
        for (auto &s : stmt->statements->list)
        {
            execute(s);
            if (breakSet || returnSet || Lox::hadRuntimeError)
            {
                break;
            }
        }
        environment = savedEnvironment;
        if (returnSet)
        {
            returnSet = false;
            return returnValue;
        }
        Object nil; // TODO: return
        return nil;
    }

    void visitBlock(Block *stmt)
    {
        Environment *locals = new Environment(environment);
        executeBlock(stmt, locals);
        delete locals;
    }

    void visitIf(If *stmt)
    {
        if (isTruthy(evaluate(stmt->condition)))
        {
            execute(stmt->thenBranch);
        }
        else
        {
            execute(stmt->elseBranch);
        }
    }

    void visitWhile(While *stmt)
    {
        while (isTruthy(evaluate(stmt->condition)))
        {
            execute(stmt->body);
            if (breakSet || returnSet || Lox::hadRuntimeError)
            {
                breakSet = false;
                break;
            }
        }
    }

    void visitBreak(Break *stmt)
    {
        breakSet = true;
    }

    void visitFunction(Function *stmt)
    {
        Object declaration(new LoxFunction(stmt, environment));
        environment->define(stmt->name, declaration);
    }

    void visitReturn(Return *stmt)
    {
        returnValue = evaluate(stmt->value);
        returnSet = true;
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
                case StmtType_Block: visitBlock((Block *)stmt); break;
                case StmtType_If: visitIf((If *)stmt); break;
                case StmtType_While: visitWhile((While *)stmt); break;
                case StmtType_Break: visitBreak((Break *)stmt); break;
                case StmtType_Function: visitFunction((Function *)stmt); break;
                case StmtType_Return: visitReturn((Return *)stmt); break;
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
