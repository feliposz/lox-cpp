#include <iostream>
#include <string>
#include "../Expr.h"

std::string visit(Expr *expr);

std::string parenthesize(std::string name, Expr *expr1 = 0, Expr *expr2 = 0, Expr *expr3 = 0)
{
    std::stringstream ss;
    ss << "(" << name;
    if (expr1) ss << " " << visit(expr1);
    if (expr2) ss << " " << visit(expr2);
    if (expr3) ss << " " << visit(expr3);
    ss << ")";
    return ss.str();
}

std::string visitBinary(Binary *expr)
{
    return parenthesize(expr->oper->lexeme, expr->left, expr->right);
}

std::string visitGrouping(Grouping *expr)
{
    return parenthesize("group", expr->expression);
}

std::string visitLiteral(Literal *expr)
{
    return expr->value->str();
}

std::string visitUnary(Unary *expr)
{
    return parenthesize(expr->oper->lexeme, expr->right);
}

std::string visit(Expr *expr)
{
    switch (expr->type)
    {
        case ExprType_Binary: return visitBinary((Binary *)expr);
        case ExprType_Grouping: return visitGrouping((Grouping *)expr);
        case ExprType_Literal: return visitLiteral((Literal *)expr);
        case ExprType_Unary: return visitUnary((Unary *)expr);
    }
    return "<<<INVALID EXPR TYPE>>>";
}

int main(int argc, char *argv[])
{
    Object *nil = new Object();
    Object *o123 = new Object(123);
    Literal *l123 = new Literal(o123);
    Token *minus = new Token(MINUS, "-", nil, 1);
    Unary *um123 = new Unary(minus, l123);
    Token *star = new Token(STAR, "*", nil, 1);
    Object *o45 = new Object(45.67);
    Literal *l45 = new Literal(o45);
    Grouping *grp = new Grouping(l45);
    Expr *expr = new Binary(um123, star, grp);

    std::cout << visit(expr) << std::endl;

    // TODO: leaking memory... (use unique_ptr???)

    return EXIT_SUCCESS;
}