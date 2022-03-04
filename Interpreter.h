#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Lox.h"
#include "Expr.h"
#include "Stmt.h"
#include "Environment.h"

class Interpreter
{
    static Environment *environment;
    static std::unordered_map<Expr *, int> *locals;
    bool repl = false;
    bool breakSet = false;
    bool returnSet = false;
    Object returnValue;
public:
    static Environment *globals;
    Interpreter(bool repl);
    void runtimeError(Token oper, std::string message);
    bool isTruthy(Object value);
    bool isEqual(Object a, Object b);
    bool checkNumberOperand(Token oper, Object value);
    bool checkNumberOperands(Token oper, Object a, Object b);
    Object visitAssign(Assign *expr);
    Object visitTernary(Ternary *expr);
    Object visitBinary(Binary *expr);
    Object visitLogical(Logical *expr);
    Object visitGrouping(Grouping *expr);
    Object visitLiteral(Literal *expr);
    Object visitUnary(Unary *expr);
    Object visitVariable(Variable *expr);
    Object visitCall(Call *stmt);
    Object visitGet(Get *expr);
    Object visitSet(Set *expr);
    Object visitSuper(Super *expr);
    Object visitThis(This *expr);
    Object visitLambda(Lambda *expr);
    Object evaluate(Expr *expr);
    void visitPrint(Print *stmt);
    void visitExpression(Expression *stmt);
    void visitVar(Var *stmt);
    Object executeBlock(Block *stmt, Environment *execEnvironment);
    void visitBlock(Block *stmt);
    void visitIf(If *stmt);
    void visitWhile(While *stmt);
    void visitBreak(Break *stmt);
    void visitFunction(Function *stmt);
    void visitClass(Class *stmt);
    void visitReturn(Return *stmt);
    void execute(Stmt *stmt);
    void interpret(std::vector<Stmt *> statements);
    void resolve(Expr *expr, int depth);
    Object lookUpVariable(Token *name, Expr *expr);
};
