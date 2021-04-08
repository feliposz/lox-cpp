#pragma once

#include <vector>

class Token;

class ListToken
{
public:
    std::vector<Token *> list;
    ListToken() {}
    ~ListToken();
};
