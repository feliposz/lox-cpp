#pragma once

#include <string>

class LoxClass
{
    std::string name;

public:
    LoxClass(std::string name) : name(name) {}
    
    std::string str()
    {
        return name;
    }
};
