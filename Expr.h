#pragma once

#include "Token.h"

enum ExprType
{
    ExprType_Binary,
    ExprType_Grouping,
    ExprType_Literal,
    ExprType_Unary,
};

struct Expr
{
    ExprType type;

    Expr(ExprType type)
    {
        this->type = type;
    }
};

struct Binary : public Expr
{
    Expr *left;
    Token *oper;
    Expr *right;

    Binary(Expr *left, Token *oper, Expr *right) : Expr(ExprType_Binary)
    {
        this->left = left;
        this->oper = oper;
        this->right = right;
    }
};

struct Grouping : public Expr
{
    Expr *expression;

    Grouping(Expr *expression) : Expr(ExprType_Grouping)
    {
        this->expression = expression;
    }
};

struct Literal : public Expr
{
    Object *value;

    Literal(Object *value) : Expr(ExprType_Literal)
    {
        this->value = value;
    }
};

struct Unary : public Expr
{
    Token *oper;
    Expr *right;

    Unary(Token *oper, Expr *right) : Expr(ExprType_Unary)
    {
        this->oper = oper;
        this->right = right;
    }
};

