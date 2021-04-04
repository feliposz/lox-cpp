#pragma once

#include <vector>

struct Stmt;

class ListStmt
{
public:
    std::vector<Stmt *> list;
    ListStmt() {}
    ~ListStmt();
};
