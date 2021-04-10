#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Lox.h"
#include "Scanner.h"
#include "AstPrinter.h"
#include "Parser.h"
#include "Interpreter.h"
#include "Resolver.h"

bool Lox::hadError = false;
bool Lox::hadRuntimeError = false;

void Lox::runFile(char *filename)
{
    std::ifstream infile(filename, std::ios::in | std::ios::binary);
    if (!infile.bad() && infile.is_open())
    {
        std::ostringstream content;
        content << infile.rdbuf();
        infile.close();
        run(content.str(), false);
        if (hadError)
        {
            exit(65);
        }
        if (hadRuntimeError)
        {
            exit(70);
        }
    }
    else
    {
        std::cerr << "Could not open file " << filename << std::endl;
    }
}

void Lox::runPrompt()
{
    for (;;)
    {
        std::string line;
        std::cout << "> ";
        if (std::getline(std::cin, line))
        {
            run(line, true);
            hadError = false;
        }
        else
        {
            break;
        }
    }
}

void Lox::run(std::string source, bool repl)
{
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();
    Parser parser(tokens);
    std::vector<Stmt *> statements = parser.parse();
    if (!hadError)
    {
        Interpreter interpreter(repl);
        Resolver resolver(&interpreter);
        resolver.resolve(statements);
        if (!hadError)
        {
            interpreter.interpret(statements);
        }
    }
    if (!repl)
    {
        for (auto &statement : statements)
        {
            if (statement->type != StmtType_Function) // HACK: Must keep original function statement alive in REPL mode...
            {
                delete statement;
            }
        }
    }
}

void Lox::error(int line, std::string message)
{
    report(line, "", message);
}

void Lox::error(Token token, std::string message)
{
    if (token.type == EOF_TOKEN)
    {
        report(token.line, " at end", message);
    }
    else
    {
        std::stringstream ss;
        ss << " at '" << token.lexeme << "'";
        report(token.line, ss.str(), message);
    }
}

void Lox::runtimeError(Token token, std::string message)
{
    std::cerr << message << std::endl << "[line " << token.line << "]" << std::endl;
    hadRuntimeError = true;
}

void Lox::report(int line, std::string where, std::string message)
{
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    hadError = true;
}