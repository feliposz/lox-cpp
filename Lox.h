#pragma once

class Lox
{
public:
    static bool hadError;

    static void runFile(char *filename)
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

    static void runPrompt()
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

    static void run(std::string source)
    {
        Scanner scanner(source);
        std::list<Token> tokens = scanner.scanTokens();

        for (auto &token : tokens)
        {
            std::cout << token << std::endl;
        }
    }

    static void error(int line, std::string message)
    {
        report(line, "", message);
    }

    static void report(int line, std::string where, std::string message)
    {
        std::cerr << "[line " << line << "] Error " << where << ": " << message << std::endl;
        hadError = true;
    }
};

bool Lox::hadError = false;