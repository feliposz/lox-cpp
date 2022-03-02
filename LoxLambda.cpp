#include "LoxLambda.h"
#include "Environment.h"
#include "Interpreter.h"

int LoxLambda::arity()
{
    return declaration->params->list.size();
}

Object LoxLambda::call(Interpreter *interpreter, std::vector<Object> arguments)
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
    return value;
}

std::string LoxLambda::str()
{
    return "<lambda fn>";
}
