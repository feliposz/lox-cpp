#pragma once

#include <vector>
#include <unordered_map>
#include "Lox.h"
#include "Expr.h"
#include "Stmt.h"
#include "Interpreter.h"

enum FunctionType
{
    FunctionType_None,
    FunctionType_Function,
};

class Resolver
{
    std::vector<std::unordered_map<std::string, bool> *> scopes;
    Interpreter *interpreter;
    FunctionType currentFunction;

    void beginScope();
    void endScope();
    void declare(Token *name);
    void define(Token *name);
    void resolveLocal(Expr *expr, Token *name);
    void visitAssign(Assign *expr);
    void visitTernary(Ternary *expr);
    void visitBinary(Binary *expr);
    void visitLogical(Logical *expr);
    void visitGrouping(Grouping *expr);
    void visitLiteral(Literal *expr);
    void visitUnary(Unary *expr);
    void visitVariable(Variable *expr);
    void visitCall(Call *stmt);
    void resolve(Expr *expr);
    void visitPrint(Print *stmt);
    void visitExpression(Expression *stmt);
    void visitVar(Var *stmt);
    void visitBlock(Block *stmt);
    void visitIf(If *stmt);
    void visitWhile(While *stmt);
    void visitBreak(Break *stmt);
    void visitFunction(Function *stmt);
    void resolveFunction(Function *stmt, FunctionType type);
    void visitReturn(Return *stmt);
    void resolve(Stmt *stmt);
public:
    Resolver(Interpreter *interpreter);
    void resolve(std::vector<Stmt *> statements);
};
