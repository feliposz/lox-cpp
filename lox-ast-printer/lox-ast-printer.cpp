#include "AstPrinter.h"

int main(int argc, char *argv[])
{
    Object *nil = new Object();
    Object *o123 = new Object(123.0);
    Literal *l123 = new Literal(o123);
    Token *minus = new Token(MINUS, "-", nil, 1);
    Unary *um123 = new Unary(minus, l123);
    Token *star = new Token(STAR, "*", nil, 1);
    Object *o45 = new Object(45.67);
    Literal *l45 = new Literal(o45);
    Grouping *grp = new Grouping(l45);
    Expr *expr = new Binary(um123, star, grp);

    AstPrinter::print(expr);

    delete expr;

    return EXIT_SUCCESS;
}