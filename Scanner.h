#pragma once
#include <string>
#include <list>
#include "Token.h"

class Scanner
{
private:
    unsigned int start = 0;
    unsigned int current = 0;
    unsigned int line = 1;
    std::string source;
    std::list<Token> tokens;
public:
    Scanner(std::string source_);
    std::list<Token> scanTokens();
    bool isAtEnd();
    void scanToken();
    char advance();
    void addToken(TokenType type);
    void addString(std::string str);
    bool match(char expected);
    char peek();
    void string();
};