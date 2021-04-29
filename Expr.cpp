#include "Expr.h"
#include "Stmt.h"

Assign::~Assign()
{
    delete name;
    delete value;
}

Ternary::~Ternary()
{
    delete first;
    delete oper1;
    delete second;
    delete oper2;
    delete third;
}

Binary::~Binary()
{
    delete left;
    delete oper;
    delete right;
}

Logical::~Logical()
{
    delete left;
    delete oper;
    delete right;
}

Grouping::~Grouping()
{
    delete expression;
}

Literal::~Literal()
{
    delete value;
}

Set::~Set()
{
    delete object;
    delete name;
    delete value;
}

Unary::~Unary()
{
    delete oper;
    delete right;
}

Variable::~Variable()
{
    delete name;
}

Call::~Call()
{
    delete callee;
    delete paren;
    delete arguments;
}

Get::~Get()
{
    delete object;
    delete name;
}

Lambda::~Lambda()
{
    delete keyword;
    delete params;
    delete body;
}
