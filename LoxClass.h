#pragma once

#include <string>
#include <unordered_map>
#include "LoxCallable.h"
#include "LoxFunction.h"

class LoxClass : public LoxCallable
{
public:
    std::string name;
    std::unordered_map<std::string, LoxFunction*> *methods;
    std::unordered_map<std::string, LoxFunction*> *statics;

    LoxClass(std::string name, std::unordered_map<std::string, LoxFunction*> *methods, std::unordered_map<std::string, LoxFunction*> *statics)
        : name(name), methods(methods), statics(statics) {}
    
    std::string str()
    {
        return name;
    }

    LoxFunction* findStatic(std::string name)
    {
        auto it = statics->find(name);
        if (it != statics->end())
        {
            return it->second;
        }
        return nullptr;
    }

    LoxFunction* findMethod(std::string name)
    {
        auto it = methods->find(name);
        if (it != methods->end())
        {
            return it->second;
        }
        return nullptr;
    }

    virtual int arity() override;
    virtual Object call(Interpreter * interpreter, std::vector<Object>) override;
};
