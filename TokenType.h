#pragma once

enum TokenType
{
    // Single-character tokens: ( ) [ ] , . - + ; / *
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    QUESTION, COLON,

    // One or two character tokens: ! != = == > >= < <=
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals: ... "..." +/-12345.67890
    IDENTIFIER, STRING, NUMBER,

    // Keywords: and class else false fun for if nil or print return super this true var while
    AND, CLASS, ELSE, FALSE_TOKEN, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE_TOKEN, VAR, WHILE, BREAK,

    EOF_TOKEN,
};
