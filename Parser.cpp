#include "Parser.h"
#include "Lox.h"

Parser::Parser(std::vector<Token> tokens)
{
    this->tokens = tokens;
}

std::vector<Stmt *> Parser::parse()
{
    std::vector<Stmt *> result;
    while (!isAtEnd())
    {
        Stmt *stmt  = declaration();
        if (Lox::hadError)
        {
            synchronize();
            if (stmt)
            {
                delete stmt;
            }
            stmt = nullptr;
            break;
        }
        else
        {
            result.push_back(stmt);
        }
    }
    return result;
}

Stmt * Parser::declaration()
{
    if (match(FUN))
    {
        return (Stmt *)function("function");
    }
    else if (match(CLASS))
    {
        return classDeclaration();
    }
    else if (match(VAR))
    {
        return varDeclaration();
    }
    return statement();
}

Stmt * Parser::varDeclaration()
{
    if (consume(IDENTIFIER, "Expect variable name."))
    {
        Token *name = new Token(previous());

        Expr *initializer = nullptr;
        if (match(EQUAL))
        {
            initializer = expression();
        }

        if (consume(SEMICOLON, "Expect ';' after variable declaration."))
        {
            return new Var(name, initializer);
        }

        delete initializer;
        delete name;
    }
    return nullptr;
}

void * Parser::function(std::string kind)
{
    Token *name = nullptr;
    Token *keyword = nullptr;
    if (kind == "lambda")
    {
        keyword = new Token(previous());
    }
    else if (check(IDENTIFIER))
    {
        consume(IDENTIFIER, "Expect " + kind + " name.");
        name = new Token(previous());
    }
    if (consume(LEFT_PAREN, "Expect '(' after " + kind + " name."))
    {
        ListToken *params = new ListToken();
        if (!check(RIGHT_PAREN))
        {
            do
            {
                if (consume(IDENTIFIER, "Expect parameter name."))
                {
                    params->list.push_back(new Token(previous()));
                }
                else
                {
                    break;
                }
            } while (match(COMMA));
        }
        if (consume(RIGHT_PAREN, "Expect ')' after parameters."))
        {
            if (consume(LEFT_BRACE, "Expect '{' before " + kind + " body."))
            {
                Block *body = (Block *)blockStatement();
                if (kind == "lambda")
                {
                    return new Lambda(keyword, params, body);
                }
                else
                {
                    
                    return new Function(name, params, body);
                }
            }
        }
        delete params;
    }
    if (name)
    {
        delete name;
    }
    if (keyword)
    {
        delete keyword;
    }
    return nullptr;
}

Stmt * Parser::classDeclaration()
{
    if (consume(IDENTIFIER, "Expect class name."))
    {
        Token *name = new Token(previous());
        if (consume(LEFT_BRACE, "Expect '{' before class body."))
        {
            ListFunction *methods = new ListFunction();
            while (!check(RIGHT_BRACE) && !isAtEnd())
            {
                Function *fn = (Function *)function("method");
                if (fn)
                {
                    methods->list.push_back(fn);
                }
                else
                {
                    delete methods;
                    delete name;
                    return nullptr;
                }
            }
            if (consume(RIGHT_BRACE, "Expect '}' after class body."))
            {
                return new Class(name, methods);
            }
            delete methods;
        }
        delete name;
    }
    return nullptr;
}

Stmt * Parser::statement()
{
    if (match(PRINT))
    {
        return printStatement();
    }
    else if (match(LEFT_BRACE))
    {
        return blockStatement();
    }
    else if (match(IF))
    {
        return ifStatement();
    }
    else if (match(WHILE))
    {
        return whileStatement();
    }
    else if (match(FOR))
    {
        return forStatement();
    }
    else if (match(BREAK))
    {
        return breakStatement();
    }
    else if (match(RETURN))
    {
        return returnStatement();
    }
    return expressionStatement();
}

Stmt * Parser::printStatement()
{
    Expr *expr = expression();
    if (consume(SEMICOLON, "Expected ';' after value."))
    {
        return new Print(expr);
    }
    else
    {
        delete expr;
        return nullptr;
    }
}

Stmt * Parser::expressionStatement()
{
    Expr *expr = expression();
    if (consume(SEMICOLON, "Expected ';' after value."))
    {
        return new Expression(expr);
    }
    else
    {
        delete expr;
        return nullptr;
    }
}

Stmt * Parser::blockStatement()
{
    ListStmt *stmts = new ListStmt();

    while (!check(RIGHT_BRACE) && !isAtEnd() && !Lox::hadError)
    {
        stmts->list.emplace_back(declaration());
    }

    consume(RIGHT_BRACE, "Expect '}' after block.");
    return new Block(stmts);
}

Stmt * Parser::ifStatement()
{
    consume(LEFT_PAREN, "Expected '(' after 'if'.");
    Expr *expr = expression();
    consume(RIGHT_PAREN, "Expected ')' after expression.");
    
    Stmt *thenStmt = statement();
    Stmt *elseStmt = nullptr;
    if (match(ELSE))
    {
        elseStmt = statement();
    }

    return new If(expr, thenStmt, elseStmt);
}

Stmt * Parser::whileStatement()
{
    consume(LEFT_PAREN, "Expected '(' after 'while'.");
    Expr *expr = expression();
    consume(RIGHT_PAREN, "Expected ')' after expression.");
    loopDepth++;
    Stmt *body = statement();
    loopDepth--;
    return new While(expr, body);
}

Stmt * Parser::forStatement()
{
    consume(LEFT_PAREN, "Expected '(' after 'for'.");

    Stmt *initializer;
    if (match(SEMICOLON))
    {
        initializer = nullptr;
    }
    else if (match(VAR))
    {
        initializer = varDeclaration();
    }
    else
    {
        initializer = expressionStatement();
    }

    Expr *condition;
    if (check(SEMICOLON))
    {
        condition = nullptr;
    }
    else 
    {
        condition = expression();
    }
    consume(SEMICOLON, "Expected ';' after for condition.");

    Expr *increment;
    if (check(RIGHT_PAREN))
    {
        increment = nullptr;
    }
    else
    {
        increment = expression();
    }
    consume(RIGHT_PAREN, "Expected ')' after for increment.");

    loopDepth++;
    Stmt *body = statement();
    loopDepth--;

    if (increment)
    {
        ListStmt *stmts = new ListStmt();
        stmts->list.push_back(body);
        stmts->list.push_back(new Expression(increment));
        body = new Block(stmts);
    }

    if (!condition)
    {
        condition = new Literal(new Object(true));
    }
    body = new While(condition, body);

    if (initializer)
    {
        ListStmt *stmts = new ListStmt();
        stmts->list.push_back(initializer);
        stmts->list.push_back(body);
        body = new Block(stmts);
    }

    return body;
}

Stmt * Parser::breakStatement()
{
    Token *keyword = new Token(previous());
    consume(SEMICOLON, "Expected ';' after 'break'.");
    if (loopDepth == 0)
    {
        Lox::runtimeError(*keyword, "'break' statement not enclosed by while/for loop.");
    }
    return new Break(keyword);
}

Stmt * Parser::returnStatement()
{
    Token *keyword = new Token(previous());
    Expr *value = nullptr;
    if (!check(SEMICOLON))
    {
        value = expression();
    }
    consume(SEMICOLON, "Expected ';' after 'return'.");
    return new Return(keyword, value);
}

Expr * Parser::expression()
{
    Expr *expr = assignment();
    return expr;
}

Expr * Parser::assignment()
{
    Expr *expr = conditional();

    if (match(EQUAL))
    {
        Token equals = previous();

        if (expr->type == ExprType_Variable)
        {
            Expr *value = assignment();
            Variable *variable = (Variable *)expr;
            Token *name = new Token(*variable->name);
            delete expr;
            return new Assign(name, value);
        }
        else if (expr->type == ExprType_Get)
        {
            Get *get = (Get *)expr;
            Expr *value = assignment();
            return new Set(get->object, get->name, value);
        }

        error(equals, "Invalid assign target.");
    }

    return expr;
}

Expr * Parser::conditional()
{
    Expr *expr = logic_or();

    if (match(QUESTION))
    {
        Token *question = new Token(previous());
        Expr *second = conditional();
        if (match(COLON))
        {
            Token *colon = new Token(previous());
            Expr *third = conditional();
            expr = new Ternary(expr, question, second, colon, third);
        }
        else
        {
            error(peek(), "Expected ':'");
            delete question;
            delete second;
        }
    }

    return expr;
}

Expr * Parser::logic_or()
{
    Expr *expr = logic_and();

    while (match(OR))
    {
        Token *oper = new Token(previous());
        Expr *right = logic_and();
        expr = new Logical(expr, oper, right);
    }

    return expr;
}

Expr * Parser::logic_and()
{
    Expr *expr = equality();

    while (match(AND))
    {
        Token *oper = new Token(previous());
        Expr *right = equality();
        expr = new Logical(expr, oper, right);
    }

    return expr;
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
    if (match(MINUS) || match(BANG) || match(STAR) || match(SLASH) || match(PLUS))
    {
        if ((previous().type == MINUS) || (previous().type == BANG))
        {
            Token *oper = new Token(previous());
            Expr *right = unary();
            return new Unary(oper, right);
        }
        else
        {
            error(previous(), "Missing left operand.");
            return nullptr;
        }
    }
    return call();
}

Expr * Parser::call()
{
    Expr *expr = primary();

    while (true)
    {
        if (match(LEFT_PAREN))
        {
            expr = finishCall(expr);
        }
        else if (match(DOT))
        {
            if (consume(IDENTIFIER, "Expect property name after '.'."))
            {
                expr = new Get(expr, new Token(previous()));
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return expr;
}

Expr * Parser::finishCall(Expr *callee)
{
    ListExpr *arguments = new ListExpr();

    if (!check(RIGHT_PAREN))
    {
        do
        {
            if (arguments->list.size() >= 255)
            {
                error(peek(), "Can't have more than 255 arguments.");
            }
            arguments->list.push_back(expression());
        } while (match(COMMA));
    }
    consume(RIGHT_PAREN, "Expected ')' after arguments.");

    return new Call(callee, new Token(previous()), arguments);
}

Expr * Parser::primary()
{
    if (match(TRUE_TOKEN))
    {
        return new Literal(new Object(true));
    }
    else if (match(FALSE_TOKEN))
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
        if (consume(RIGHT_PAREN, "Expected ')' after expression."))
        {
            return new Grouping(expr);
        }
        else
        {
            // Error ocurred, allow to recover
            return expr;
        }
    }
    else if (match(THIS))
    {
        return new This(new Token(previous()));
    }
    else if (match(IDENTIFIER))
    {
        return new Variable(new Token(previous()));
    }
    else if (match(FUN))
    {
        return (Expr *)function("lambda");
    }
    else if (isAtEnd())
    {
        return nullptr;
    }
    error(peek(), "Expected expression.");
    return nullptr;
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
    return peek().type == EOF_TOKEN;
}

bool Parser::consume(TokenType type, std::string message)
{
    if (check(type))
    {
        advance();
        return true;
    }
    error(peek(), message);
    return false;
}

void Parser::error(Token token, std::string message)
{
    Lox::error(token, message);
}

void Parser::synchronize()
{
    advance();

    while (!isAtEnd())
    {
        if (previous().type == SEMICOLON)
        {
            return;
        }

        switch (peek().type)
        {
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
                return;
        }

        advance();
    }
}
