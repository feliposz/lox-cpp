#pragma once
class Expr
{
    // 
}

class Binary : public Expr
{
    // Expr left, Token operator, Expr right
}

class Grouping : public Expr
{
    // Expr expression
}

class Literal : public Expr
{
    // Object value
}

class Unary : public Expr
{
    // Token operator, Expr right
}

