#include <sstream>
#include "Token.h"

static const char *TokenTypeNames[] = {
    "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE",
    "COMMA", "DOT", "MINUS", "PLUS", "SEMICOLON", "SLASH", "STAR",
    "QUESTION", "COLON",

    "BANG", "BANG_EQUAL",
    "EQUAL", "EQUAL_EQUAL",
    "GREATER", "GREATER_EQUAL",
    "LESS", "LESS_EQUAL",

    "IDENTIFIER", "STRING", "NUMBER",

    "AND", "CLASS", "ELSE", "FALSE", "FUN", "FOR", "IF", "NIL", "OR",
    "PRINT", "RETURN", "SUPER", "THIS", "TRUE", "VAR", "WHILE", "BREAK",

    "EOF_TOKEN",
};

Token::Token(TokenType type_, std::string lexeme_, Object *literal_, int line_)
{
    type = type_;
    lexeme = lexeme_;
    literal = *literal_;
    line = line_;
}

std::string Token::str()
{
    std::ostringstream s;
    s << TokenTypeNames[type] << " " << lexeme << " " << literal.str();
    return s.str();
}

std::ostream &operator<<(std::ostream &os, Token &t)
{
    os << t.str();
    return os;
}