#pragma once

#include "Stmt.h"
#include "LoxCallable.h"

class Environment;

class LoxFunction : public LoxCallable
{
private:
    Function *declaration;
    Environment *closure;

public:
    LoxFunction(Function *declaration, Environment *closure)
    {
        this->declaration = declaration;
        this->closure = closure;
    }

    // Inherited via Callable
    virtual int arity() override;
    virtual Object call(Interpreter *interpreter, std::vector<Object>) override;
    virtual std::string str() override;
};
