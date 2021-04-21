#pragma once

#include <vector>

struct Function;

class ListFunction
{
public:
    std::vector<Function *> list;
    ListFunction() {}
    ~ListFunction();
};
