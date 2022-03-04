#pragma once

#include "Token.h"
#include "ListToken.h"
#include "ListExpr.h"
struct Block;

enum ExprType
{
    ExprType_Assign,
    ExprType_Ternary,
    ExprType_Binary,
    ExprType_Logical,
    ExprType_Grouping,
    ExprType_Literal,
    ExprType_Set,
    ExprType_Super,
    ExprType_This,
    ExprType_Unary,
    ExprType_Variable,
    ExprType_Call,
    ExprType_Get,
    ExprType_Lambda,
};

struct Expr
{
    ExprType type;
    Expr(ExprType type) : type(type) {}
    virtual ~Expr() {}
};

struct Assign : public Expr
{
    Token *name;
    Expr *value;

    Assign(Token *name, Expr *value) : Expr(ExprType_Assign), name(name), value(value) {}

    ~Assign();
};

struct Ternary : public Expr
{
    Expr *first;
    Token *oper1;
    Expr *second;
    Token *oper2;
    Expr *third;

    Ternary(Expr *first, Token *oper1, Expr *second, Token *oper2, Expr *third) : Expr(ExprType_Ternary), first(first), oper1(oper1), second(second), oper2(oper2), third(third) {}

    ~Ternary();
};

struct Binary : public Expr
{
    Expr *left;
    Token *oper;
    Expr *right;

    Binary(Expr *left, Token *oper, Expr *right) : Expr(ExprType_Binary), left(left), oper(oper), right(right) {}

    ~Binary();
};

struct Logical : public Expr
{
    Expr *left;
    Token *oper;
    Expr *right;

    Logical(Expr *left, Token *oper, Expr *right) : Expr(ExprType_Logical), left(left), oper(oper), right(right) {}

    ~Logical();
};

struct Grouping : public Expr
{
    Expr *expression;

    Grouping(Expr *expression) : Expr(ExprType_Grouping), expression(expression) {}

    ~Grouping();
};

struct Literal : public Expr
{
    Object *value;

    Literal(Object *value) : Expr(ExprType_Literal), value(value) {}

    ~Literal();
};

struct Set : public Expr
{
    Expr *object;
    Token *name;
    Expr *value;

    Set(Expr *object, Token *name, Expr *value) : Expr(ExprType_Set), object(object), name(name), value(value) {}

    ~Set();
};

struct Super : public Expr
{
    Token *keyword;
    Token *method;

    Super(Token *keyword, Token *method) : Expr(ExprType_Super), keyword(keyword), method(method) {}

    ~Super();
};

struct This : public Expr
{
    Token *keyword;

    This(Token *keyword) : Expr(ExprType_This), keyword(keyword) {}

    ~This();
};

struct Unary : public Expr
{
    Token *oper;
    Expr *right;

    Unary(Token *oper, Expr *right) : Expr(ExprType_Unary), oper(oper), right(right) {}

    ~Unary();
};

struct Variable : public Expr
{
    Token *name;

    Variable(Token *name) : Expr(ExprType_Variable), name(name) {}

    ~Variable();
};

struct Call : public Expr
{
    Expr *callee;
    Token *paren;
    ListExpr *arguments;

    Call(Expr *callee, Token *paren, ListExpr *arguments) : Expr(ExprType_Call), callee(callee), paren(paren), arguments(arguments) {}

    ~Call();
};

struct Get : public Expr
{
    Expr *object;
    Token *name;

    Get(Expr *object, Token *name) : Expr(ExprType_Get), object(object), name(name) {}

    ~Get();
};

struct Lambda : public Expr
{
    Token *keyword;
    ListToken *params;
    Block *body;

    Lambda(Token *keyword, ListToken *params, Block *body) : Expr(ExprType_Lambda), keyword(keyword), params(params), body(body) {}

    ~Lambda();
};

