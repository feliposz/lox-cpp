#pragma once

#include <list>
#include <string>
#include "Object.h"

class Callable
{
public:
    virtual int arity() = 0;
    virtual Object call(std::list<Object>) = 0;
    virtual std::string str() = 0;
};