#pragma once

class Token
{
public:
    TokenType type;
    std::string lexeme;
    std::string strLiteral;
    double numLiteral;
    int line;

    Token(TokenType type_, std::string lexeme_, std::string strLiteral_, double numLiteral_, int line_)
    {
        type = type_;
        lexeme = lexeme_;
        strLiteral = strLiteral_;
        numLiteral = numLiteral_;
        line = line_;
    }

    std::string str()
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
};

std::ostream &operator<<(std::ostream &os, Token &t)
{
    os << t.str();
    return os;
}