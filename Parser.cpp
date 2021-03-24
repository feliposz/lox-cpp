#include "Parser.h"
#include "Lox.h"

Parser::Parser(std::vector<Token> tokens)
{
    this->tokens = tokens;
}

Expr * Parser::parse()
{
    try
    {
        return expression();
    }
    catch (ParseError& e)
    {
        return nullptr;
    }
}

Expr * Parser::expression()
{
    return equality();
}

Expr * Parser::equality()
{
    Expr *expr = comparison();

    while (match(EQUAL_EQUAL) || match(BANG_EQUAL))
    {
        Token *oper = new Token(previous());
        Expr *right = comparison();
        expr = new Binary(expr, oper, right);
    }

    return expr;
}

Expr * Parser::comparison()
{
    Expr *expr = term();

    while (match(LESS) || match(GREATER) || match(LESS_EQUAL) || match(GREATER_EQUAL))
    {        
        Token *oper = new Token(previous());
        Expr *right = term();
        expr = new Binary(expr, oper, right);
    }

    return expr;
}

Expr * Parser::term()
{
    Expr *expr = factor();

    while (match(MINUS) || match(PLUS))
    {
        Token *oper = new Token(previous());
        Expr *right = factor();
        expr = new Binary(expr, oper, right);
    }

    return expr;
}

Expr * Parser::factor()
{
    Expr *expr = unary();

    while (match(STAR) || match(SLASH))
    {
        Token *oper = new Token(previous());
        Expr *right = unary();
        expr = new Binary(expr, oper, right);
    }

    return expr;
}

Expr * Parser::unary()
{
    if (match(MINUS) || match(BANG))
    {
        Token *oper = new Token(previous());
        Expr *right = unary();
        return new Unary(oper, right);
    }
    return primary();
}

Expr * Parser::primary()
{
    if (match(TRUE))
    {
        return new Literal(new Object(true));
    }
    else if (match(FALSE))
    {
        return new Literal(new Object(false));
    }
    else if (match(NIL))
    {
        return new Literal(new Object());
    }
    else if (match(NUMBER))
    {
        Token t = previous();
        return new Literal(new Object(t.literal));
    }
    else if (match(STRING))
    {
        Token t = previous();
        return new Literal(new Object(t.literal));
    }
    else if (match(LEFT_PAREN))
    {
        Expr *expr = expression();
        consume(RIGHT_PAREN, "Expected ')' after expression.");
        return new Grouping(expr);
    }
    
}

bool Parser::match(TokenType type)
{
    if (check(type))
    {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type)
{
    if (isAtEnd())
    {
        return false;
    }
    return peek().type == type;
}

Token Parser::advance()
{
    if (!isAtEnd())
    {
        return tokens[current++];
    }
    return previous();
}

Token Parser::peek()
{
    return tokens[current];
}

Token Parser::previous()
{
    return tokens[current - 1];
}

bool Parser::isAtEnd()
{
    return peek().type == EOF;
}

Token Parser::consume(TokenType type, std::string message)
{
    if (check(type))
    {
        return advance();
    }
    throw error(peek(), message);
}

ParseError *Parser::error(Token token, std::string message)
{
    Lox::error(token, message);
    return new ParseError();
}
