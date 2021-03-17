#include <iostream>
#include "Lox.h"

#define EXIT_USAGE 64

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        std::cout << "Usage: lox [script]" << std::endl;
        return EXIT_USAGE;
    }
    else if (argc == 2)
    {
        Lox::runFile(argv[1]);
    }
    else
    {
        Lox::runPrompt();
    }
    return EXIT_SUCCESS;
}
