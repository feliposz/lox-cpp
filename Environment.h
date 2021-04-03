#pragma once

#include <unordered_map>
#include <string>

class Environment
{
private:
    std::unordered_map<std::string, Object> values;

public:

    void define(Token *name, Object value)
    {
        // TODO: check redefinition?
        values.emplace(name->lexeme, value);
    }

    void assign(Token *name, Object value)
    {
        auto it = values.find(name->lexeme);

        if (it != values.end())
        {
            it->second = value;
        }
        else
        {
            Lox::runtimeError(*name, "Undefined variable '" + name->lexeme + "'");
        }        
    }

    Object get(Token *name)
    {
        auto it = values.find(name->lexeme);

        if (it != values.end())
        {
            return it->second;
        }

        Lox::runtimeError(*name, "Undefined variable '" + name->lexeme + "'");
        Object nil;
        return nil;
    }

};