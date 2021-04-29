#include "LoxClass.h"
#include "LoxInstance.h"

int LoxClass::arity()
{
    return 0;
}

Object LoxClass::call(Interpreter * interpreter, std::vector<Object>)
{
    return Object(new LoxInstance(this));
}
