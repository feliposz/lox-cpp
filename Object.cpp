#include <sstream>
#include "Object.h"
#include "LoxCallable.h"
#include "LoxClass.h"
#include "LoxInstance.h"

Object::~Object()
{
    if (function)
    {
        //TODO: delete function;
        //TODO: Consider using a shared ptr?
    }
}

std::string Object::str()
{
    if (type == TYPE_STRING)
    {
        return strLiteral;
    }
    else if (type == TYPE_NUMBER)
    {
        std::ostringstream s;
        s << numLiteral;
        return s.str();
    }
    else if (type == TYPE_NIL)
    {
        return "nil";
    }
    else if (type == TYPE_BOOL)
    {
        return boolLiteral ? "true" : "false";
    }
    else if (type == TYPE_FUNCTION)
    {
        return function->str();
    }
    else if (type == TYPE_CLASS)
    {
        return loxClass->str();
    }
    else if (type == TYPE_INSTANCE)
    {
        return loxInstance->str();
    }
    else
    {
        return "<<OBJECT STRING NOT IMPLEMENTED>>";
    }
}