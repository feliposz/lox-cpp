#pragma once

#include <vector>
#include <string>
#include "Object.h"

class Interpreter;

class LoxCallable
{
public:
    virtual int arity() = 0;
    virtual Object call(Interpreter *interpreter, std::vector<Object>) = 0;
    virtual std::string str() = 0;
};