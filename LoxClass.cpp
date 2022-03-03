#include "LoxClass.h"
#include "LoxInstance.h"

int LoxClass::arity()
{
    LoxFunction *initializer = findMethod("init");
    if (initializer)
    {
        return initializer->arity();
    }
    return 0;
}

Object LoxClass::call(Interpreter * interpreter, std::vector<Object> arguments)
{
    LoxInstance *instance = new LoxInstance(this);
    LoxFunction *initializer = findMethod("init");
    if (initializer)
    {
        LoxFunction *init = initializer->bind(instance);
        init->call(interpreter, arguments);
        delete init;
    }
    return Object(instance);
}
