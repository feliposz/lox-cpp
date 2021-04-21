#include "Stmt.h"
#include "ListFunction.h"

ListFunction::~ListFunction()
{
    for (Function *function : list)
    {
        delete function;
    }
}
