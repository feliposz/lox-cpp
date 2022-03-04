#include "Interpreter.h"
#include "NativeFunctions.h"
#include "LoxLambda.h"
#include "LoxClass.h"
#include "LoxInstance.h"

Environment *Interpreter::environment;
Environment *Interpreter::globals;
std::unordered_map<Expr *, int> *Interpreter::locals;

Interpreter::Interpreter(bool repl)
{
    this->repl = repl;
    if (!environment)
    {
        globals = new Environment();
        environment = globals;
        locals = new std::unordered_map<Expr *, int>();
        Object clock(new ClockFunction());
        environment->define("clock", clock);
    }
}

void Interpreter::runtimeError(Token oper, std::string message)
{
    Lox::runtimeError(oper, message);
}

bool Interpreter::isTruthy(Object value)
{
    switch (value.type)
    {
        case TYPE_NIL:
            return false;
        case TYPE_BOOL:
            return value.boolLiteral;
        default:
            return true;
    }
}

bool Interpreter::isEqual(Object a, Object b)
{
    if (a.type != b.type)
    {
        return false;
    }

    switch (a.type)
    {
        case TYPE_NIL: return true;
        case TYPE_BOOL: return a.boolLiteral == b.boolLiteral;
        case TYPE_NUMBER: return a.numLiteral == b.numLiteral;
        case TYPE_STRING: return a.strLiteral == b.strLiteral;
        case TYPE_FUNCTION: return a.function == b.function;
        case TYPE_CLASS: return a.loxClass == b.loxClass;
        case TYPE_INSTANCE: return a.loxInstance == b.loxInstance;
    }

    return false;
}

bool Interpreter::checkNumberOperand(Token oper, Object value)
{
    if (value.type == TYPE_NUMBER)
    {
        return true;
    }
    else
    {
        runtimeError(oper, "Operand must be a number.");
        return false;
    }
}

bool Interpreter::checkNumberOperands(Token oper, Object a, Object b)
{
    if (a.type == TYPE_NUMBER && b.type == TYPE_NUMBER)
    {
        return true;
    }
    else
    {
        runtimeError(oper, "Operands must be numbers.");
        return false;
    }
}

Object Interpreter::visitAssign(Assign *expr)
{
    Object value = evaluate(expr->value);

    auto it = locals->find(expr);
    if (it != locals->end())
    {
        int distance = it->second;
        environment->defineAt(distance, expr->name, value);
    }
    else
    {
        globals->assign(expr->name, value);
    }
    return value;
}

Object Interpreter::visitTernary(Ternary *expr)
{
    Object cond = evaluate(expr->first);

    if (isTruthy(cond))
    {
        return evaluate(expr->second);
    }
    else
    {
        return evaluate(expr->third);
    }
}

Object Interpreter::visitBinary(Binary *expr)
{
    Object left = evaluate(expr->left);
    Object right = evaluate(expr->right);

    switch (expr->oper->type)
    {
        case MINUS:
        {
            checkNumberOperands(*expr->oper, left, right);
            Object value(left.numLiteral - right.numLiteral);
            return value;
        }

        case STAR:
        {
            checkNumberOperands(*expr->oper, left, right);
            Object value(left.numLiteral * right.numLiteral);
            return value;
        }

        case SLASH:
        {
            checkNumberOperands(*expr->oper, left, right);
            if (right.numLiteral == 0)
            {
                runtimeError(*expr->oper, "Divide by zero.");
            }
            Object value(left.numLiteral / right.numLiteral);
            return value;
        }

        case PLUS:
        {
            if (left.type == TYPE_NUMBER && right.type == TYPE_NUMBER)
            {
                Object value(left.numLiteral + right.numLiteral);
                return value;
            }
            else if (left.type == TYPE_STRING && right.type == TYPE_STRING)
            {
                Object value(left.strLiteral + right.strLiteral);
                return value;
            }
            else if (left.type == TYPE_STRING)
            {
                Object value(left.strLiteral + right.str());
                return value;
            }
            else if (right.type == TYPE_STRING)
            {
                Object value(left.str() + right.strLiteral);
                return value;
            }
            else
            {
                runtimeError(*expr->oper, "Operands must be both numbers or at least one should be a string.");
            }
        }

        case EQUAL_EQUAL:
        {
            Object value(isEqual(left, right));
            return value;
        }

        case BANG_EQUAL:
        {
            Object value(!isEqual(left, right));
            return value;
        }

        case LESS:
        {
            if (left.type == TYPE_NUMBER && right.type == TYPE_NUMBER)
            {
                Object value(left.numLiteral < right.numLiteral);
                return value;
            }
            else if (left.type == TYPE_STRING && right.type == TYPE_STRING)
            {
                Object value(left.strLiteral < right.strLiteral);
                return value;
            }
            else
            {
                runtimeError(*expr->oper, "Operands must be both strings or both numbers.");
            }
        }

        case GREATER:
        {
            if (left.type == TYPE_NUMBER && right.type == TYPE_NUMBER)
            {
                Object value(left.numLiteral > right.numLiteral);
                return value;
            }
            else if (left.type == TYPE_STRING && right.type == TYPE_STRING)
            {
                Object value(left.strLiteral > right.strLiteral);
                return value;
            }
            else
            {
                runtimeError(*expr->oper, "Operands must be both strings or both numbers.");
            }
        }

        case LESS_EQUAL:
        {
            if (left.type == TYPE_NUMBER && right.type == TYPE_NUMBER)
            {
                Object value(left.numLiteral <= right.numLiteral);
                return value;
            }
            else if (left.type == TYPE_STRING && right.type == TYPE_STRING)
            {
                Object value(left.strLiteral <= right.strLiteral);
                return value;
            }
            else
            {
                runtimeError(*expr->oper, "Operands must be both strings or both numbers.");
            }
        }

        case GREATER_EQUAL:
        {
            if (left.type == TYPE_NUMBER && right.type == TYPE_NUMBER)
            {
                Object value(left.numLiteral >= right.numLiteral);
                return value;
            }
            else if (left.type == TYPE_STRING && right.type == TYPE_STRING)
            {
                Object value(left.strLiteral >= right.strLiteral);
                return value;
            }
            else
            {
                runtimeError(*expr->oper, "Operands must be both strings or both numbers.");
            }
        }

        default:
            runtimeError(*expr->oper, "Invalid binary operator.");
    }

    return left;
}

Object Interpreter::visitLogical(Logical *expr)
{
    Object left = evaluate(expr->left);

    if (expr->oper->type == AND)
    {
        if (isTruthy(left))
        {
            return evaluate(expr->right);
        }
    }
    else
    {
        if (!isTruthy(left))
        {
            return evaluate(expr->right);
        }
    }

    return left;
}

Object Interpreter::visitGrouping(Grouping *expr)
{
    Object value = evaluate(expr->expression);
    return value;
}

Object Interpreter::visitLiteral(Literal *expr)
{
    return *expr->value;
}

Object Interpreter::visitUnary(Unary *expr)
{
    Object right = evaluate(expr->right);

    switch (expr->oper->type)
    {
        case MINUS:
        {
            checkNumberOperand(*expr->oper, right);
            Object value(-right.numLiteral);
            return value;
        }

        case BANG:
        {
            Object value(!isTruthy(right));
            return value;
        }

        default:
            runtimeError(*expr->oper, "Invalid unary operator.");
    }

    return right;
}

Object Interpreter::visitVariable(Variable *expr)
{
    return lookUpVariable(expr->name, expr);
}

Object Interpreter::visitCall(Call *stmt)
{
    Object callee = evaluate(stmt->callee);

    std::vector<Object> arguments;
    for (auto &argument : stmt->arguments->list)
    {
        arguments.push_back(evaluate(argument));
    }

    if ((callee.type != TYPE_FUNCTION) && (callee.type != TYPE_CLASS))
    {
        runtimeError(*stmt->paren, "Can only call functions and methods.");
    }
    else
    {
        LoxCallable *callable = callee.type == TYPE_CLASS ? callee.loxClass : callee.function;
        if (callable->arity() != arguments.size())
        {
            std::stringstream ss;
            ss << "Expected " << callable->arity() << " arguments but got " << arguments.size() << ".";
            runtimeError(*stmt->paren, ss.str());
            return callee;
        }
        else
        {
            Object result = callable->call(this, arguments);
            returnSet = false;
            return result;
        }
    }
    return callee;
}

Object Interpreter::visitGet(Get *expr)
{
    Object obj = evaluate(expr->object);
    if (obj.type == TYPE_INSTANCE)
    {
        return obj.loxInstance->get(expr->name);
    }
    else if (obj.type == TYPE_CLASS)
    {
        LoxFunction *fn = obj.loxClass->findStatic(expr->name->lexeme);
        if (fn)
        {
            return Object(fn);
        }
        runtimeError(*expr->name, "Class has no such static method.");
    }
    runtimeError(*expr->name, "Only instances have properties.");
    return Object();
}

Object Interpreter::visitSet(Set *expr)
{
    Object obj = evaluate(expr->object);
    if (obj.type == TYPE_INSTANCE)
    {
        Object value = evaluate(expr->value);
        obj.loxInstance->set(expr->name, value);
        return value;
    }
    runtimeError(*expr->name, "Only instances have properties.");
    return Object();
}

Object Interpreter::visitThis(This *expr)
{
    return lookUpVariable(expr->keyword, expr);
}

Object Interpreter::visitLambda(Lambda *expr)
{
    Object lambda(new LoxLambda(expr, environment));
    return lambda;
}

Object Interpreter::evaluate(Expr *expr)
{
    if (expr)
    {
        switch (expr->type)
        {
            case ExprType_Assign: return visitAssign((Assign *)expr);
            case ExprType_Ternary: return visitTernary((Ternary *)expr);
            case ExprType_Binary: return visitBinary((Binary *)expr);
            case ExprType_Logical: return visitLogical((Logical *)expr);
            case ExprType_Grouping: return visitGrouping((Grouping *)expr);
            case ExprType_Literal: return visitLiteral((Literal *)expr);
            case ExprType_Unary: return visitUnary((Unary *)expr);
            case ExprType_Variable: return visitVariable((Variable *)expr);
            case ExprType_Call: return visitCall((Call *)expr);
            case ExprType_Get: return visitGet((Get *)expr);
            case ExprType_Set: return visitSet((Set *)expr);
            case ExprType_This: return visitThis((This *)expr);
            case ExprType_Lambda: return visitLambda((Lambda *)expr);
            default: Lox::error(0, "Invalid expression type.");
        }
    }    
    Object nil;
    return nil;
}

void Interpreter::visitPrint(Print *stmt)
{
    Object value = evaluate(stmt->expression);
    if (!Lox::hadRuntimeError)
    {
        std::cout << value.str() << std::endl;
    }
}

void Interpreter::visitExpression(Expression *stmt)
{
    Object value = evaluate(stmt->expression);
    if (repl && !Lox::hadRuntimeError)
    {
        std::cout << value.str() << std::endl;
    }
}

void Interpreter::visitVar(Var *stmt)
{
    Object value;
    if (stmt->initializer)
    {
        value = evaluate(stmt->initializer);
    }
    else
    {
        value.type = TYPE_UNINITIALIZED;
    }
    environment->define(stmt->name, value);
}

Object Interpreter::executeBlock(Block *stmt, Environment *execEnvironment)
{
    Environment *savedEnvironment = environment;
    environment = execEnvironment;
    for (auto &s : stmt->statements->list)
    {
        execute(s);
        if (breakSet || returnSet || Lox::hadRuntimeError)
        {
            break;
        }
    }
    environment = savedEnvironment;
    if (returnSet)
    {
        return returnValue;
    }
    Object nil;
    return nil;
}

void Interpreter::visitBlock(Block *stmt)
{
    Environment locals(environment);
    executeBlock(stmt, &locals);
}

void Interpreter::visitIf(If *stmt)
{
    if (isTruthy(evaluate(stmt->condition)))
    {
        execute(stmt->thenBranch);
    }
    else
    {
        execute(stmt->elseBranch);
    }
}

void Interpreter::visitWhile(While *stmt)
{
    while (isTruthy(evaluate(stmt->condition)))
    {
        execute(stmt->body);
        if (breakSet || returnSet || Lox::hadRuntimeError)
        {
            breakSet = false;
            break;
        }
    }
}

void Interpreter::visitBreak(Break *stmt)
{
    breakSet = true;
}

void Interpreter::visitFunction(Function *stmt)
{
    Object declaration(new LoxFunction(stmt, environment, false));
    environment->define(stmt->name, declaration);
}

void Interpreter::visitClass(Class *stmt)
{
    std::unordered_map<std::string, LoxFunction*>* methods = new std::unordered_map<std::string, LoxFunction*>();
    for (const auto &method : stmt->methods->list)
    {
        LoxFunction* function = new LoxFunction(method, environment, method->name->lexeme == "init");
        methods->emplace(method->name->lexeme, function);
    }

    std::unordered_map<std::string, LoxFunction*>* statics = new std::unordered_map<std::string, LoxFunction*>();
    for (const auto &method : stmt->statics->list)
    {
        LoxFunction* function = new LoxFunction(method, environment, false);
        statics->emplace(method->name->lexeme, function);
    }

    Object superclass;
    if (stmt->superclass)
    {
        superclass = evaluate(stmt->superclass);
        if (superclass.type != TYPE_CLASS)
        {
            Lox::runtimeError(*stmt->superclass->name, "Superclass must be a class.");
            return;
        }
    }

    Object loxClass(new LoxClass(stmt->name->lexeme, superclass.loxClass, methods, statics));
    environment->define(stmt->name, loxClass);
}

void Interpreter::visitReturn(Return *stmt)
{
    returnValue = evaluate(stmt->value);
    returnSet = true;
}

void Interpreter::execute(Stmt *stmt)
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

void Interpreter::interpret(std::vector<Stmt *> statements)
{
    for (auto &statement : statements)
    {
        if (statement)
        {
            execute(statement);
        }
        if (Lox::hadRuntimeError)
        {
            Lox::hadRuntimeError = false;
            break;
        }
        else if (Lox::hadError)
        {
            break;
        }
    }
}

void Interpreter::resolve(Expr * expr, int depth)
{
    (*locals)[expr] = depth;
}

Object Interpreter::lookUpVariable(Token *name, Expr *expr)
{
    auto it = locals->find(expr);
    if (it != locals->end())
    {
        int distance = it->second;
        return environment->getAt(distance, name);
    }
    else
    {
        return globals->get(name);
    }
}