#pragma once

#include "Token.h"

enum ExprType
{
    ExprType_Assign,
    ExprType_Ternary,
    ExprType_Binary,
    ExprType_Grouping,
    ExprType_Literal,
    ExprType_Unary,
    ExprType_Variable,
};

struct Expr
{
    ExprType type;

    Expr(ExprType type)
    {
        this->type = type;
    }

    virtual ~Expr()
    {
    }
};

struct Assign : public Expr
{
    Token *name;
    Expr *value;

    Assign(Token *name, Expr *value) : Expr(ExprType_Assign)
    {
        this->name = name;
        this->value = value;
    }

    ~Assign()
    {
        delete name;
        delete value;
    }
};

struct Ternary : public Expr
{
    Expr *first;
    Token *oper1;
    Expr *second;
    Token *oper2;
    Expr *third;

    Ternary(Expr *first, Token *oper1, Expr *second, Token *oper2, Expr *third) : Expr(ExprType_Ternary)
    {
        this->first = first;
        this->oper1 = oper1;
        this->second = second;
        this->oper2 = oper2;
        this->third = third;
    }

    ~Ternary()
    {
        delete first;
        delete oper1;
        delete second;
        delete oper2;
        delete third;
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

    ~Binary()
    {
        delete left;
        delete oper;
        delete right;
    }
};

struct Grouping : public Expr
{
    Expr *expression;

    Grouping(Expr *expression) : Expr(ExprType_Grouping)
    {
        this->expression = expression;
    }

    ~Grouping()
    {
        delete expression;
    }
};

struct Literal : public Expr
{
    Object *value;

    Literal(Object *value) : Expr(ExprType_Literal)
    {
        this->value = value;
    }

    ~Literal()
    {
        delete value;
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

    ~Unary()
    {
        delete oper;
        delete right;
    }
};

struct Variable : public Expr
{
    Token *name;

    Variable(Token *name) : Expr(ExprType_Variable)
    {
        this->name = name;
    }

    ~Variable()
    {
        delete name;
    }
};

