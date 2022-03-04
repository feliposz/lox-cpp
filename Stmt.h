#pragma once

#include "Expr.h"
#include "ListStmt.h"
#include "ListToken.h"
#include "ListFunction.h"

enum StmtType
{
    StmtType_Expression,
    StmtType_Print,
    StmtType_Var,
    StmtType_Block,
    StmtType_Function,
    StmtType_Class,
    StmtType_If,
    StmtType_While,
    StmtType_Return,
    StmtType_Break,
};

struct Stmt
{
    StmtType type;
    Stmt(StmtType type) : type(type) {}
    virtual ~Stmt() {}
};

struct Expression : public Stmt
{
    Expr *expression;

    Expression(Expr *expression) : Stmt(StmtType_Expression), expression(expression) {}

    ~Expression();
};

struct Print : public Stmt
{
    Expr *expression;

    Print(Expr *expression) : Stmt(StmtType_Print), expression(expression) {}

    ~Print();
};

struct Var : public Stmt
{
    Token *name;
    Expr *initializer;

    Var(Token *name, Expr *initializer) : Stmt(StmtType_Var), name(name), initializer(initializer) {}

    ~Var();
};

struct Block : public Stmt
{
    ListStmt *statements;

    Block(ListStmt *statements) : Stmt(StmtType_Block), statements(statements) {}

    ~Block();
};

struct Function : public Stmt
{
    Token *name;
    ListToken *params;
    Block *body;

    Function(Token *name, ListToken *params, Block *body) : Stmt(StmtType_Function), name(name), params(params), body(body) {}

    ~Function();
};

struct Class : public Stmt
{
    Token *name;
    Variable *superclass;
    ListFunction *methods;
    ListFunction *statics;

    Class(Token *name, Variable *superclass, ListFunction *methods, ListFunction *statics) : Stmt(StmtType_Class), name(name), superclass(superclass), methods(methods), statics(statics) {}

    ~Class();
};

struct If : public Stmt
{
    Expr *condition;
    Stmt *thenBranch;
    Stmt *elseBranch;

    If(Expr *condition, Stmt *thenBranch, Stmt *elseBranch) : Stmt(StmtType_If), condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}

    ~If();
};

struct While : public Stmt
{
    Expr *condition;
    Stmt *body;

    While(Expr *condition, Stmt *body) : Stmt(StmtType_While), condition(condition), body(body) {}

    ~While();
};

struct Return : public Stmt
{
    Token *keyword;
    Expr *value;

    Return(Token *keyword, Expr *value) : Stmt(StmtType_Return), keyword(keyword), value(value) {}

    ~Return();
};

struct Break : public Stmt
{
    Token *keyword;

    Break(Token *keyword) : Stmt(StmtType_Break), keyword(keyword) {}

    ~Break();
};

