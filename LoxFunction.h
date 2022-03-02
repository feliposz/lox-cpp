#pragma once

#include "Stmt.h"
#include "LoxCallable.h"
#include "Environment.h"

class LoxFunction : public LoxCallable
{
private:
    Function *declaration;
    Environment *closure;
    bool isInitializer;

public:
    LoxFunction(Function *declaration, Environment *closure, bool isInitializer)
    {
        this->declaration = declaration;
        this->closure = closure;
        this->isInitializer = isInitializer;
    }

    LoxFunction* bind(LoxInstance *instance)
    {
        Environment *environment = new Environment(closure);
        environment->define("this", Object(instance));
        return new LoxFunction(declaration, environment, isInitializer);
    }

    // Inherited via Callable
    virtual int arity() override;
    virtual Object call(Interpreter *interpreter, std::vector<Object>) override;
    virtual std::string str() override;
};
