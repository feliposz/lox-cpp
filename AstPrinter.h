#pragma once

#include <iostream>
#include <string>
#include "Expr.h"

namespace AstPrinter
{
    std::string visit(Expr *expr);

    std::string parenthesize(std::string name, Expr *expr1 = 0, Expr *expr2 = 0, Expr *expr3 = 0, Expr *expr4 = 0)
    {
        std::stringstream ss;
        ss << "(" << name;
        if (expr1) ss << " " << visit(expr1);
        if (expr2) ss << " " << visit(expr2);
        if (expr3) ss << " " << visit(expr3);
        if (expr4) ss << " " << visit(expr4);
        ss << ")";
        return ss.str();
    }

    std::string visitTernary(Ternary *expr)
    {
        return parenthesize(expr->oper1->lexeme + expr->oper2->lexeme, expr->first, expr->second, expr->third);
    }

    std::string visitBinary(Binary *expr)
    {
        return parenthesize(expr->oper->lexeme, expr->left, expr->right);
    }

    std::string visitGrouping(Grouping *expr)
    {
        return parenthesize("group", expr->expression);
    }

    std::string visitLiteral(Literal *expr)
    {
        return expr->value->str();
    }

    std::string visitUnary(Unary *expr)
    {
        return parenthesize(expr->oper->lexeme, expr->right);
    }

    std::string visit(Expr *expr)
    {
        switch (expr->type)
        {
            case ExprType_Ternary: return visitTernary((Ternary *)expr);
            case ExprType_Binary: return visitBinary((Binary *)expr);
            case ExprType_Grouping: return visitGrouping((Grouping *)expr);
            case ExprType_Literal: return visitLiteral((Literal *)expr);
            case ExprType_Unary: return visitUnary((Unary *)expr);
        }
        return "<<<INVALID EXPR TYPE>>>";
    }

    void print(Expr *expr)
    {
        std::cout << "::AstPrinter:: " <<  visit(expr) << std::endl;
    }

};