#pragma once

#include <vector>

struct Expr;

class ListExpr
{
public:
    std::vector<Expr *> list;
    ListExpr() {}
    ~ListExpr();
};
