#pragma once

#include <unordered_map>
#include <string>

class Environment
{
private:
    std::unordered_map<std::string, Object> values;

public:

    void define(std::string name, Object value)
    {
        values.emplace(name, value);
    }

    Object get(Token *identifier)
    {
        auto it = values.find(identifier->lexeme);

        if (it != values.end())
        {
            return it->second;
        }

        Lox::runtimeError(*identifier, "Undeclared identifier.");
        Object nil;
        return nil;
    }

};