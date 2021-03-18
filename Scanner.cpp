#include <unordered_map>
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
        case '{': addToken(LEFT_BRACE); break;
        case '}': addToken(RIGHT_BRACE); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case '-': addToken(MINUS); break;
        case '+': addToken(PLUS); break;
        case ';': addToken(SEMICOLON); break;
        case '/':
            if (match('/'))
            {
                while ((peek() != '\n') && !isAtEnd())
                {
                    advance();
                }
            }
            else
            {
                addToken(SLASH);
            }
        break;        
        case '*': addToken(STAR); break;
        case '!': addToken(match('=') ? BANG_EQUAL : BANG); break;
        case '=': addToken(match('=') ? EQUAL_EQUAL : EQUAL); break;
        case '>': addToken(match('=') ? GREATER_EQUAL : GREATER); break;
        case '<': addToken(match('=') ? LESS_EQUAL : LESS); break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line++;
            break;
        case '"':
            string();
            break;
        default:
            if (isDigit(c))
            {
                number();
            }
            else if (isAlpha(c))
            {
                identifier();
            }
            else
            {
                Lox::error(line, "Invalid character");
            }
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

bool Scanner::match(char expected)
{
    if (isAtEnd())
    {
        return false;
    }
    else if (source[current] != expected)
    {
        return false;
    }
    current++;
    return true;
}

char Scanner::peek()
{
    if (isAtEnd())
    {
        return '\0';
    }
    return source[current];
}

char Scanner::peekNext()
{
    if ((current + 1) >= source.length())
    {
        return '\0';
    }
    return source[current + 1];
}

void Scanner::addString(std::string str)
{
    std::string substr = source.substr(start, current - start);
    Token token(STRING, substr, str, 0, line);
    tokens.push_back(token);
}

void Scanner::addNumber(double num)
{
    std::string substr = source.substr(start, current - start);
    Token token(NUMBER, substr, "", num, line);
    tokens.push_back(token);
}

void Scanner::string()
{
    while ((peek() != '"') && !isAtEnd())
    {
        if (peek() == '\n')
        {
            line++;
        }
        advance();
    }

    if (isAtEnd())
    {
        Lox::error(line, "Unterminated string");
        return;
    }

    advance(); // closing "

    // remove enclosing ""
    std::string str = source.substr(start + 1, current - start - 2);
    addString(str);
}

bool Scanner::isDigit(char c)
{
    return (c >= '0') && (c <= '9');
}

bool Scanner::isAlpha(char c)
{
    return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || (c == '_');
}

bool Scanner::isAlphaNum(char c)
{
    return isDigit(c) || isAlpha(c);
}

void Scanner::number()
{
    while (isDigit(peek()))
    {
        advance();
    }

    if ((peek() == '.') && isDigit(peekNext()))
    {
        advance();

        while (isDigit(peek()))
        {
            advance();
        }
    }

    std::string str = source.substr(start, current - start);
    double num = stod(str);
    addNumber(num);
}

static std::unordered_map<std::string, TokenType> keywords({
    { "and", AND },
    { "class", CLASS },
    { "else", ELSE },
    { "false", FALSE },
    { "for", FOR },
    { "fun", FUN },
    { "if", IF },
    { "nil", NIL },
    { "or", OR },
    { "print", PRINT },
    { "return", RETURN },
    { "super", SUPER },
    { "this", THIS },
    { "true", TRUE },
    { "var", VAR },
    { "while", WHILE }
});

void Scanner::identifier()
{
    while (isAlphaNum(peek()))
    {
        advance();
    }

    TokenType type;
    std::string str = source.substr(start, current - start);
    auto it = keywords.find(str);

    if (it != keywords.end())
    {
        type = it->second;
    }
    else
    {
        type = IDENTIFIER;
    }
    addToken(type);
}
