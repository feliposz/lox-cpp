#pragma once

#include "Expr.h"
#include "LoxCallable.h"

class Environment;

class LoxLambda : public LoxCallable
{
private:
    Lambda * declaration;
    Environment *closure;

public:
    LoxLambda(Lambda *declaration, Environment *closure): declaration(declaration), closure(closure) {}

    // Inherited via Callable
    virtual int arity() override;
    virtual Object call(Interpreter *interpreter, std::vector<Object>) override;
    virtual std::string str() override;
};
