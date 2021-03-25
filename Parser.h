#pragma once
#include <stdexcept>
#include <vector>
#include "Token.h"
#include "Expr.h"

class ParseError : public std::exception
{
};

class Parser
{
private:
    unsigned int current = 0;
    std::vector<Token> tokens;
    Expr *expression();
    Expr *equality();
    Expr *comparison();
    Expr *term();
    Expr *factor();
    Expr *unary();
    Expr *primary();
    bool match(TokenType type);
    bool check(TokenType type);
    Token advance();
    Token peek();
    Token previous();
    bool isAtEnd();
    bool consume(TokenType type, std::string message);
    void error(Token token, std::string message);
public:
    Parser(std::vector<Token> tokens);
    Expr *parse();
};