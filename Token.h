#pragma once
#include <string>
#include <sstream>
#include "TokenType.h"

enum ObjectType
{
    TYPE_UNINITIALIZED,
    TYPE_NIL,
    TYPE_NUMBER,
    TYPE_STRING,
    TYPE_BOOL,
};

class Object
{
public:
    ObjectType type;
    std::string strLiteral;
    double numLiteral;
    bool boolLiteral;

    Object()
    {
        type = TYPE_NIL;
    }

    Object(double num)
    {
        type = TYPE_NUMBER;
        numLiteral = num;
    }

    Object(std::string str)
    {
        type = TYPE_STRING;
        strLiteral = str;
    }

    Object(bool b)
    {
        type = TYPE_BOOL;
        boolLiteral = b;
    }

    std::string str()
    {
        if (type == TYPE_STRING)
        {
            return strLiteral;
        }
        else if (type == TYPE_NUMBER)
        {
            std::ostringstream s;
            s << numLiteral;
            return s.str();
        } 
        else if (type == TYPE_NIL)
        {
            return "nil";
        }
        else if (type == TYPE_BOOL)
        {
            return boolLiteral ? "true" : "false";
        }
        else
        {
            return "<<OBJECT STRING NOT IMPLEMENTED>>";
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