#include "Stmt.h"

Expression::~Expression()
{
    delete expression;
}

Print::~Print()
{
    delete expression;
}

Var::~Var()
{
    delete name;
    delete initializer;
}

Block::~Block()
{
    delete statements;
}

Function::~Function()
{
    delete name;
    delete params;
    delete body;
}

Class::~Class()
{
    delete name;
    delete superclass;
    delete methods;
    delete statics;
}

If::~If()
{
    delete condition;
    delete thenBranch;
    delete elseBranch;
}

While::~While()
{
    delete condition;
    delete body;
}

Return::~Return()
{
    delete keyword;
    delete value;
}

Break::~Break()
{
    delete keyword;
}
