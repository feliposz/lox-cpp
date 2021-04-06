#pragma once

#include "Expr.h"
#include "ListStmt.h"

enum StmtType
{
    StmtType_Expression,
    StmtType_Print,
    StmtType_Var,
    StmtType_Block,
    StmtType_If,
    StmtType_While,
    StmtType_Break,
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

struct Var : public Stmt
{
    Token *name;
    Expr *initializer;

    Var(Token *name, Expr *initializer) : Stmt(StmtType_Var)
    {
        this->name = name;
        this->initializer = initializer;
    }

    ~Var()
    {
        delete name;
        delete initializer;
    }
};

struct Block : public Stmt
{
    ListStmt *statements;

    Block(ListStmt *statements) : Stmt(StmtType_Block)
    {
        this->statements = statements;
    }

    ~Block()
    {
        delete statements;
    }
};

struct If : public Stmt
{
    Expr *condition;
    Stmt *thenBranch;
    Stmt *elseBranch;

    If(Expr *condition, Stmt *thenBranch, Stmt *elseBranch) : Stmt(StmtType_If)
    {
        this->condition = condition;
        this->thenBranch = thenBranch;
        this->elseBranch = elseBranch;
    }

    ~If()
    {
        delete condition;
        delete thenBranch;
        delete elseBranch;
    }
};

struct While : public Stmt
{
    Expr *condition;
    Stmt *body;

    While(Expr *condition, Stmt *body) : Stmt(StmtType_While)
    {
        this->condition = condition;
        this->body = body;
    }

    ~While()
    {
        delete condition;
        delete body;
    }
};

struct Break : public Stmt
{

    Break() : Stmt(StmtType_Break)
    {
    }

    ~Break()
    {
    }
};

