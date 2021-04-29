#pragma once

#include <string>

class LoxCallable;
class LoxClass;
class LoxInstance;

enum ObjectType
{
    TYPE_UNINITIALIZED,
    TYPE_NIL,
    TYPE_NUMBER,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_FUNCTION,
    TYPE_CLASS,
    TYPE_INSTANCE,
};

class Object
{
public:
    ObjectType type;
    std::string strLiteral;
    double numLiteral;
    bool boolLiteral;
    LoxCallable *function;
    LoxClass *loxClass;
    LoxInstance *loxInstance;

    Object()
    {
        type = TYPE_NIL;
    }

    Object(double num)
    {
        type = TYPE_NUMBER;
        numLiteral = num;
    }

    Object(std::string str)
    {
        type = TYPE_STRING;
        strLiteral = str;
    }

    Object(bool b)
    {
        type = TYPE_BOOL;
        boolLiteral = b;
    }

    Object(LoxCallable *function)
    {
        type = TYPE_FUNCTION;
        this->function = function;
    }

    Object(LoxClass *loxClass)
    {
        type = TYPE_CLASS;
        this->loxClass = loxClass;
    }

    Object(LoxInstance *loxInstance)
    {
        type = TYPE_INSTANCE;
        this->loxInstance = loxInstance;
    }

    ~Object();

    std::string str();
};

