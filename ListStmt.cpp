#include "Stmt.h"
#include "ListStmt.h"

ListStmt::~ListStmt()
{
    for (Stmt *stmt : list)
    {
        delete stmt;
    }
}
