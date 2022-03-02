#include "LoxFunction.h"
#include "Environment.h"
#include "Interpreter.h"

int LoxFunction::arity()
{
    return declaration->params->list.size();
}

Object LoxFunction::call(Interpreter *interpreter, std::vector<Object> arguments)
{
    Environment *environment = new Environment(closure);
    for (size_t i = 0; i < declaration->params->list.size(); i++)
    {
        environment->define(declaration->params->list[i], arguments[i]);
    }
    Object value = interpreter->executeBlock(declaration->body, environment);
    if (value.type != TYPE_FUNCTION)
    {
        // HACK: Find out a better/safer way to support closures?
        delete environment;
    }
    if (isInitializer)
    {
        return closure->getAt(0, "this");
    }
    return value;
}

std::string LoxFunction::str()
{
    return "<fn " + declaration->name->lexeme + ">";
}
