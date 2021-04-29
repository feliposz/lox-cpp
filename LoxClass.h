#pragma once

#include <string>
#include "LoxCallable.h"

class LoxClass : public LoxCallable
{
public:
    std::string name;

    LoxClass(std::string name) : name(name) {}
    
    std::string str()
    {
        return name;
    }

    virtual int arity() override;
    virtual Object call(Interpreter * interpreter, std::vector<Object>) override;
};
