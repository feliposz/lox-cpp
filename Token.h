#pragma once
#include <string>
#include <sstream>
#include "TokenType.h"

class Object
{
private:
    TokenType type;
    std::string strLiteral;
    double numLiteral;
public:
    Object()
    {
        type = NIL;
    }

    Object(double num)
    {
        type = NUMBER;
        numLiteral = num;
    }

    Object(std::string str)
    {
        type = STRING;
        strLiteral = str;
    }

    std::string str()
    {
        if (type == STRING)
        {
            return strLiteral;
        }
        else if (type == NUMBER)
        {
            std::ostringstream s;
            s << numLiteral;
            return s.str();
        } 
        else
        {
            return "";
        }
    }
};

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