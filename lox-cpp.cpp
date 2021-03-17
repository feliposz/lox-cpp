#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cerrno>

#define EXIT_USAGE 64

void run(std::string s)
{
    std::cout << s << std::endl;
}

void runFile(char *filename)
{
    std::ifstream infile(filename, std::ios::in | std::ios::binary);
    if (!infile.bad())
    {
        std::ostringstream content;
        content << infile.rdbuf();
        infile.close();
        run(content.str());
    }
}

void runPrompt()
{
    for (;;)
    {
        std::string line;
        std::cout << "> ";
        if (std::getline(std::cin, line))
        {
            run(line);
        }
        else
        {
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        std::cout << "Usage: lox [script]" << std::endl;
        return EXIT_USAGE;
    }
    else if (argc == 2)
    {
        runFile(argv[1]);
    }
    else
    {
        runPrompt();
    }
    return EXIT_SUCCESS;
}
