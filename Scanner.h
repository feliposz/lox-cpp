#pragma once
#include <string>
#include <vector>
#include "Token.h"

class Scanner
{
private:
    unsigned int start = 0;
    unsigned int current = 0;
    unsigned int line = 1;
    std::string source;
    std::vector<Token> tokens;
public:
    Scanner(std::string source_);
    std::vector<Token> scanTokens();
private:
    bool isAtEnd();
    void scanToken();
    char advance();
    void addToken(TokenType type);
    void addString(std::string str);
    void addNumber(double num);
    bool match(char expected);
    char peek();
    char peekNext();
    void string();
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNum(char c);
    void number();
    void identifier();
    void blockComment();
};