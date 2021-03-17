#pragma once

class Scanner
{
public:
    std::string source;
    std::list<Token> tokens;

    Scanner(std::string source_)
    {
        source = source_;
    }

    std::list<Token> scanTokens()
    {
        Token token(STRING, source, source, 0, 0);
        tokens.push_back(token);
        return tokens;
    }
};