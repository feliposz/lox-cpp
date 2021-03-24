#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Lox.h"
#include "Scanner.h"
#include "AstPrinter.h"
#include "Parser.h"

bool Lox::hadError = false;

void Lox::runFile(char *filename)
{
    std::ifstream infile(filename, std::ios::in | std::ios::binary);
    if (!infile.bad())
    {
        std::ostringstream content;
        content << infile.rdbuf();
        infile.close();
        run(content.str());
        if (hadError)
        {
            exit(65);
        }
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
            run(line);
            hadError = false;
        }
        else
        {
            break;
        }
    }
}

void Lox::run(std::string source)
{
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

#if 1
    Parser parser(tokens);

    Expr *expr = parser.parse();
    if (expr)
    {
        AstPrinter::print(expr);
        delete expr;
    }
#else
    for (auto &token : tokens)
    {
        std::cout << token << std::endl;
    }
#endif
}

void Lox::error(int line, std::string message)
{
    report(line, "", message);
}

void Lox::error(Token token, std::string message)
{
    if (token.type == EOF)
    {
        report(token.line, " at end", message);
    }
    else
    {
        // TODO: format => at 'lexeme'
        report(token.line, token.lexeme, message);
    }
}

void Lox::report(int line, std::string where, std::string message)
{
    std::cerr << "[line " << line << "] Error " << where << ": " << message << std::endl;
    hadError = true;
}