#pragma once
#include <vector>
#include "Token.h"
#include "Expr.h"
#include "Stmt.h"

class Parser
{
private:
    unsigned int current = 0;
    unsigned int loopDepth = 0;
    std::vector<Token> tokens;
    Stmt *declaration();
    Stmt *varDeclaration();
    void *function(std::string kind);
    Stmt *classDeclaration();
    Stmt *statement();
    Stmt *printStatement();
    Stmt *expressionStatement();
    Stmt *blockStatement();
    Stmt *ifStatement();
    Stmt *whileStatement();
    Stmt *forStatement();
    Stmt *breakStatement();
    Stmt *returnStatement();
    Expr *expression();
    Expr *assignment();
    Expr *conditional();
    Expr *logic_or();
    Expr *logic_and();
    Expr *equality();
    Expr *comparison();
    Expr *term();
    Expr *factor();
    Expr *unary();
    Expr *call();
    Expr *finishCall(Expr *callee);
    Expr *primary();
    bool match(TokenType type);
    bool check(TokenType type);
    Token advance();
    Token peek();
    Token previous();
    bool isAtEnd();
    bool consume(TokenType type, std::string message);
    void error(Token token, std::string message);
    void synchronize();
public:
    Parser(std::vector<Token> tokens);
    std::vector<Stmt *> parse();
};