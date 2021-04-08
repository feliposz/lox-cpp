#include "LoxFunction.h"
#include "Environment.h"
#include "Interpreter.h"

int LoxFunction::arity()
{
    return declaration->params->list.size();
}

Object LoxFunction::call(Interpreter *interpreter, std::vector<Object> arguments)
{
    Environment *environment = new Environment(interpreter->globals);
    for (int i = 0; i < declaration->params->list.size(); i++)
    {
        environment->define(declaration->params->list[i], arguments[i]);
    }
    Object value = interpreter->executeBlock(declaration->body, environment);
    delete environment;
    return value;
}

std::string LoxFunction::str()
{
    return "<fn " + declaration->name->lexeme + ">";
}
