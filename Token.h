#pragma once
#include <string>
#include "TokenType.h"

class Token
{
public:
    TokenType type;
    std::string lexeme;
    std::string strLiteral;
    double numLiteral;
    int line;

    Token(TokenType type_, std::string lexeme_, std::string strLiteral_, double numLiteral_, int line_);
    std::string str();
};

std::ostream &operator<<(std::ostream &os, Token &t);