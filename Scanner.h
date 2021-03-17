#pragma once

class Scanner
{
private:
    unsigned int start = 0;
    unsigned int current = 0;
    unsigned int line = 1;
public:
    std::string source;
    std::list<Token> tokens;

    Scanner(std::string source_)
    {
        source = source_;
    }

    std::list<Token> scanTokens()
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

    bool isAtEnd()
    {
        return current >= source.length();
    }

    void scanToken()
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
        }
    }

    char advance()
    {
        return source[current++];
    }

    void addToken(TokenType type)
    {
        std::string substr = source.substr(start, current - start);
        Token token(type, substr, "", 0, line);
        tokens.push_back(token);
    }
};