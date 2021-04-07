#pragma once
#include <string>
#include <sstream>
#include "TokenType.h"
#include "Object.h"

class Token
{
public:
    TokenType type;
    std::string lexeme;
    Object literal;
    int line;

    Token(TokenType type_, std::string lexeme_, Object *literal_, int line_);
    std::string str();
};

std::ostream &operator<<(std::ostream &os, Token &t);