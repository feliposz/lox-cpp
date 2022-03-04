#include "Resolver.h"

Resolver::Resolver(Interpreter *interpreter)
{
    this->interpreter = interpreter;
    this->currentFunction = FunctionType_None;
    this->currentClass = ClassType_None;
}

void Resolver::beginScope()
{
    scopes.push_back(new std::unordered_map<std::string, VariableFlags>());
}

void Resolver::endScope()
{
    auto scope = scopes.back();
    for (const auto &pair : *scope)
    {
        if (!pair.second.referenced)
        {
            //Lox::error(0, "Unreferenced variable: " + pair.first);
        }
    }
    delete scope;
    scopes.pop_back();
}

void Resolver::declare(Token * name)
{
    if (scopes.empty())
    {
        return;
    }

    auto scope = scopes.back();

    auto it = scope->find(name->lexeme);
    if (it != scope->end())
    {
        Lox::error(*name, "Variable already defined in this scope.");
    }
    (*scope)[name->lexeme].initialized = false;
    (*scope)[name->lexeme].referenced = false;
}

void Resolver::define(Token * name)
{
    if (scopes.empty())
    {
        return;
    }

    auto scope = scopes.back();
    (*scope)[name->lexeme].initialized = true;
}

void Resolver::resolveLocal(Expr *expr, Token *name)
{
    for (int i = scopes.size() - 1; i >= 0; i--)
    {
        auto scope = scopes[i];
        auto it = scope->find(name->lexeme);
        if (it != scope->end())
        {
            interpreter->resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }
}

void Resolver::visitAssign(Assign * expr)
{
    resolve(expr->value);
    resolveLocal(expr, expr->name);
}

void Resolver::visitTernary(Ternary * expr)
{
    resolve(expr->first);
    resolve(expr->second);
    resolve(expr->third);
}

void Resolver::visitBinary(Binary * expr)
{
    resolve(expr->left);
    resolve(expr->right);
}

void Resolver::visitLogical(Logical * expr)
{
    resolve(expr->left);
    resolve(expr->right);
}

void Resolver::visitGrouping(Grouping * expr)
{
    resolve(expr->expression);
}

void Resolver::visitLiteral(Literal * expr)
{
}

void Resolver::visitUnary(Unary * expr)
{
    resolve(expr->right);
}

void Resolver::visitVariable(Variable * expr)
{
    if (!scopes.empty())
    {
        auto scope = scopes.back();
        auto it = scope->find(expr->name->lexeme);
        if (it != scope->end())
        {
            if (!it->second.initialized)
            {
                Lox::error(*expr->name, "Can't read local variable in its own initializer.");
            }
            it->second.referenced = true;
        }
    }

    resolveLocal(expr, expr->name);
}

void Resolver::visitCall(Call * stmt)
{
    resolve(stmt->callee);
    for (auto &argument : stmt->arguments->list)
    {
        resolve(argument);
    }
}

void Resolver::visitGet(Get *expr)
{
    resolve(expr->object);
}

void Resolver::visitSet(Set *expr)
{
    resolve(expr->value);
    resolve(expr->object);
}

void Resolver::visitSuper(Super *expr)
{
    if (currentClass == ClassType_None)
    {
        Lox::error(*expr->keyword, "Can't use 'super' outside of a class.");
        return;
    }
    else if (currentClass != ClassType_Subclass)
    {
        Lox::error(*expr->keyword, "Can't use 'super' in a class with no superclass.");
        return;
    }
    resolveLocal(expr, expr->keyword);
}

void Resolver::visitThis(This *expr)
{
    if (currentClass == ClassType_None)
    {
        Lox::error(*expr->keyword, "Can't use 'this' outside of a class.");
        return;
    }
    resolveLocal(expr, expr->keyword);
}

void Resolver::visitLambda(Lambda* stmt)
{
    resolveFunction(stmt, FunctionType_Lambda);
}

void Resolver::resolve(Expr * expr)
{
    if (expr)
    {
        switch (expr->type)
        {
            case ExprType_Assign: visitAssign((Assign *)expr); break;
            case ExprType_Ternary: visitTernary((Ternary *)expr); break;
            case ExprType_Binary: visitBinary((Binary *)expr); break;
            case ExprType_Logical: visitLogical((Logical *)expr); break;
            case ExprType_Set: visitSet((Set *)expr); break;
            case ExprType_Super: visitSuper((Super *)expr); break;
            case ExprType_This: visitThis((This *)expr); break;
            case ExprType_Grouping: visitGrouping((Grouping *)expr); break;
            case ExprType_Literal: visitLiteral((Literal *)expr); break;
            case ExprType_Unary: visitUnary((Unary *)expr); break;
            case ExprType_Variable: visitVariable((Variable *)expr); break;
            case ExprType_Call: visitCall((Call *)expr); break;
            case ExprType_Get: visitGet((Get *)expr); break;
            case ExprType_Lambda: visitLambda((Lambda *)expr); break;
            default: Lox::error(0, "Invalid expression type.");
        }
    }
}

void Resolver::visitPrint(Print * stmt)
{
    resolve(stmt->expression);
}

void Resolver::visitExpression(Expression * stmt)
{
    resolve(stmt->expression);
}

void Resolver::visitVar(Var * stmt)
{
    declare(stmt->name);
    if (stmt->initializer)
    {
        resolve(stmt->initializer);
    }
    define(stmt->name);
}

void Resolver::visitBlock(Block * stmt)
{
    beginScope();
    resolve(stmt->statements->list);
    endScope();
}

void Resolver::visitIf(If * stmt)
{
    resolve(stmt->condition);
    resolve(stmt->thenBranch);
    if (stmt->elseBranch)
    {
        resolve(stmt->elseBranch);
    }
}

void Resolver::visitWhile(While * stmt)
{
    resolve(stmt->condition);
    resolve(stmt->body);
}

void Resolver::visitBreak(Break * stmt)
{
}

void Resolver::visitFunction(Function * stmt)
{
    declare(stmt->name);
    define(stmt->name);
    resolveFunction(stmt, FunctionType_Function);
}

void Resolver::visitClass(Class *stmt)
{
    ClassType enclosingClass = currentClass;
    currentClass = ClassType_Class;

    declare(stmt->name);
    define(stmt->name);

    if (stmt->superclass)
    {
        currentClass = ClassType_Subclass;
        if (stmt->name->lexeme == stmt->superclass->name->lexeme)
        {
            Lox::error(*stmt->superclass->name, "A class can't inherit from itself.");
        }
        resolve(stmt->superclass);

        beginScope();
        VariableFlags flags = { true, true };
        scopes.back()->emplace("super", flags);
    }

    beginScope();
    VariableFlags flags = { true, true };
    scopes.back()->emplace("this", flags);

    for (auto method : stmt->methods->list)
    {
        FunctionType declaration = FunctionType_Method;
        if (method->name->lexeme == "init")
        {
            declaration = FunctionType_Initializer;
        }
        resolveFunction(method, declaration);
    }

    for (auto method : stmt->statics->list)
    {
        FunctionType declaration = FunctionType_Function;
        if (method->name->lexeme == "init")
        {
            Lox::error(0, "'init' cannot be a static method");
        }
        resolveFunction(method, declaration);
    }

    endScope(); // scope for 'this'

    if (stmt->superclass)
    {
        endScope(); // scope for 'super'
    }

    currentClass = enclosingClass;
}

void Resolver::resolveFunction(void *stmt, FunctionType type)
{
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;
    beginScope();
    ListToken *params = 0;
    ListStmt *statements = 0;
    if (type == FunctionType_Function || type == FunctionType_Method || type == FunctionType_Initializer)
    {
        params = ((Function *)stmt)->params;
        statements = ((Function *)stmt)->body->statements;
    }
    else if (type == FunctionType_Lambda)
    {
        params = ((Lambda *)stmt)->params;
        statements = ((Lambda *)stmt)->body->statements;
    }
    else
    {
        Lox::error(0, "FunctionType not implemented!");
    }
    for (auto &param : params->list)
    {
        declare(param);
        define(param);
    }
    resolve(statements->list);
    endScope();
    currentFunction = enclosingFunction;
}

void Resolver::visitReturn(Return * stmt)
{
    if (this->currentFunction == FunctionType_None)
    {
        Lox::error(*stmt->keyword, "Can't return from top-level code.");
    }

    if (stmt->value)
    {
        if (this->currentFunction == FunctionType_Initializer)
        {
            Lox::error(*stmt->keyword, "Can't return a value from an initializer.");
        }

        resolve(stmt->value);
    }
}

void Resolver::resolve(std::vector<Stmt*> statements)
{
    for (auto &stmt : statements)
    {
        resolve(stmt);
    }
}

void Resolver::resolve(Stmt *stmt)
{
    if (stmt)
    {
        switch (stmt->type)
        {
            case StmtType_Print: visitPrint((Print *)stmt); break;
            case StmtType_Expression: visitExpression((Expression *)stmt); break;
            case StmtType_Var: visitVar((Var *)stmt); break;
            case StmtType_Block: visitBlock((Block *)stmt); break;
            case StmtType_If: visitIf((If *)stmt); break;
            case StmtType_While: visitWhile((While *)stmt); break;
            case StmtType_Break: visitBreak((Break *)stmt); break;
            case StmtType_Function: visitFunction((Function *)stmt); break;
            case StmtType_Class: visitClass((Class *)stmt); break;
            case StmtType_Return: visitReturn((Return *)stmt); break;
            default: Lox::error(0, "Invalid statement type.");
        }
    }
}
