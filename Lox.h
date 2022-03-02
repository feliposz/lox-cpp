#pragma once
#include <string>
#include "Token.h"

class Lox
{
public:
    static bool hadError;
    static bool hadRuntimeError;
    static void runFile(char *filename);
    static void runPipe();
    static void runPrompt();
    static void run(std::string source, bool repl);
    static void error(int line, std::string message);
    static void error(Token token, std::string message);
    static void runtimeError(Token token, std::string message);
    static void report(int line, std::string where, std::string message);
};
