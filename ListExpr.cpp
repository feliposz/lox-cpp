#include "Expr.h"
#include "ListExpr.h"

ListExpr::~ListExpr()
{
    for (Expr *Expr : list)
    {
        delete Expr;
    }
}
