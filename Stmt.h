#pragma once

#include "Expr.h"

enum StmtType
{
    StmtType_Expression,
    StmtType_Print,
};

struct Stmt
{
    StmtType type;

    Stmt(StmtType type)
    {
        this->type = type;
    }

    virtual ~Stmt()
    {
    }
};

struct Expression : public Stmt
{
    Expr *expression;

    Expression(Expr *expression) : Stmt(StmtType_Expression)
    {
        this->expression = expression;
    }

    ~Expression()
    {
        delete expression;
    }
};

struct Print : public Stmt
{
    Expr *expression;

    Print(Expr *expression) : Stmt(StmtType_Print)
    {
        this->expression = expression;
    }

    ~Print()
    {
        delete expression;
    }
};

