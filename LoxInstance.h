#pragma once

#include <string>
#include <unordered_map>
#include "LoxClass.h"
#include "Token.h"
#include "Lox.h"

class LoxInstance
{
    std::unordered_map<std::string, Object> fields;
    LoxClass *loxClass;

public:
    
    LoxInstance(LoxClass *loxClass) : loxClass(loxClass) {}
 
    std::string str()
    {
        return loxClass->name + " instance";
    }

    Object get(Token *name)
    {
        if (fields.count(name->lexeme))
        {
            return fields[name->lexeme];
        }

        LoxFunction* method = loxClass->findMethod(name->lexeme);
        if (method)
        {
            return Object(method->bind(this));
        }

        Lox::runtimeError(*name, "Undefined property '" + name->lexeme + "'.");
        return Object();
    }

    void set(Token *name, Object value)
    {
        fields[name->lexeme] = value;
    }
};