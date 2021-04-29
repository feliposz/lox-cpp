#pragma once

#include <string>
#include "LoxClass.h"

class LoxInstance
{
    LoxClass *loxClass;

public:
    
    LoxInstance(LoxClass *loxClass) : loxClass(loxClass) {}
 
    std::string str()
    {
        return loxClass->name + " instance";
    }
};