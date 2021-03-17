#include "Lox.h"
#include "Scanner.h"

Scanner::Scanner(std::string source_)
{
    source = source_;
}

std::list<Token> Scanner::scanTokens()
{
    while (!isAtEnd())
    {
        start = current;
        scanToken();
    }
    Token eofToken(EOF_TOKEN, "", "", 0, line);
    tokens.push_back(eofToken);
    return tokens;
}

bool Scanner::isAtEnd()
{
    return current >= source.length();
}

void Scanner::scanToken()
{
    char c = advance();
    switch (c)
    {
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case '[': addToken(LEFT_BRACE); break;
        case ']': addToken(RIGHT_BRACE); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case '-': addToken(MINUS); break;
        case '+': addToken(PLUS); break;
        case ';': addToken(SEMICOLON); break;
        case '/': addToken(SLASH); break;
        case '*': addToken(STAR); break;
        case '!': addToken(BANG); break;
        case '=': addToken(EQUAL); break;
        case '>': addToken(GREATER); break;
        case '<': addToken(LESS); break;
        default:
            Lox::error(line, "Invalid character");
            break;
    }
}

char Scanner::advance()
{
    return source[current++];
}

void Scanner::addToken(TokenType type)
{
    std::string substr = source.substr(start, current - start);
    Token token(type, substr, "", 0, line);
    tokens.push_back(token);
}