#pragma once

#include "Stmt.h"
#include "LoxCallable.h"

class LoxFunction : public LoxCallable
{
private:
    Function *declaration;

public:
    LoxFunction(Function *declaration)
    {
        this->declaration = declaration;
    }

    // Inherited via Callable
    virtual int arity() override;
    virtual Object call(Interpreter *interpreter, std::vector<Object>) override;
    virtual std::string str() override;
};
