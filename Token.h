#pragma once

class Token
{
public:
    std::string str()
    {
        std::string s;
        return s;
    }
};

std::ostream &operator<<(std::ostream &os, Token &t)
{
    os << t.str();
    return os;
}