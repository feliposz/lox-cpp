#pragma once

#include "Token.h"

struct Binary;
struct Grouping;
struct Literal;
struct Unary;

template <typename R>
struct Visitor
{
    R visitBinaryExpr(Binary *expr);
    R visitGroupingExpr(Grouping *expr);
    R visitLiteralExpr(Literal *expr);
    R visitUnaryExpr(Unary *expr);
};

template <typename R>
struct Expr
{

    Expr()
    {
    }

    virtual R accept(Visitor<R> visitor);
};

template <typename R>
struct Binary : public Expr<R>
{
    Expr *left;
    Token *oper;
    Expr *right;

    Binary(Expr *left, Token *oper, Expr *right)
    {
        this->left = left;
        this->oper = oper;
        this->right = right;
    }

    override accept(Visitor<R> visitor)
    {
        return visitor.visitBinaryExpr(this);
    }
};

template <typename R>
struct Grouping : public Expr<R>
{
    Expr *expression;

    Grouping(Expr *expression)
    {
        this->expression = expression;
    }

    override accept(Visitor<R> visitor)
    {
        return visitor.visitGroupingExpr(this);
    }
};

template <typename R>
struct Literal : public Expr<R>
{
    Object *value;

    Literal(Object *value)
    {
        this->value = value;
    }

    override accept(Visitor<R> visitor)
    {
        return visitor.visitLiteralExpr(this);
    }
};

template <typename R>
struct Unary<R> : public Expr<R>
{
    Token *oper;
    Expr *right;

    Unary(Token *oper, Expr *right)
    {
        this->oper = oper;
        this->right = right;
    }

    override accept(Visitor<R> visitor)
    {
        return visitor.visitUnaryExpr(this);
    }
};

