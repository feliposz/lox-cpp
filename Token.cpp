#include <sstream>
#include "Token.h"

static const char *TokenTypeNames[] = {
    "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE",
    "COMMA", "DOT", "MINUS", "PLUS", "SEMICOLON", "SLASH", "STAR",

    "BANG", "BANG_EQUAL",
    "EQUAL", "EQUAL_EQUAL",
    "GREATER", "GREATER_EQUAL",
    "LESS", "LESS_EQUAL",

    "IDENTIFIER", "STRING", "NUMBER",

    "AND", "CLASS", "ELSE", "FALSE", "FUN", "FOR", "IF", "NIL", "OR",
    "PRINT", "RETURN", "SUPER", "THIS", "TRUE", "VAR", "WHILE",

    "EOF_TOKEN",
};

Token::Token(TokenType type_, std::string lexeme_, std::string strLiteral_, double numLiteral_, int line_)
{
    type = type_;
    lexeme = lexeme_;
    strLiteral = strLiteral_;
    numLiteral = numLiteral_;
    line = line_;
}

std::string Token::str()
{
    std::ostringstream s;
    s << TokenTypeNames[type] << " " << lexeme;
    if (type == STRING)
    {
        s << " " << strLiteral;
    }
    else if (type == NUMBER)
    {
        s << " " << numLiteral;
    }
    return s.str();
}

std::ostream &operator<<(std::ostream &os, Token &t)
{
    os << t.str();
    return os;
}