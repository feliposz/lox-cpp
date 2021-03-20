#include <iostream>
#include <strstream>
#include <fstream>
#include <sstream>
#include <vector>

#define EXIT_USAGE 64

void defineType(std::ofstream &writer, const char *basename, std::string &className, std::string &fields)
{
    if (basename)
    {
        writer << "class " << className << " : public " << basename << std::endl;
    }
    else
    {
        writer << "class " << className << std::endl;
    }
    writer << "{" << std::endl;
    writer << "    // " << fields << std::endl;
    writer << "}" << std::endl << std::endl;
}

void trimString(std::string &str)
{
    str.erase(0, str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);
}

void defineAst(const char *outputDir, const char *basename, std::vector<std::string> &types)
{
    std::ostrstream path; // TODO: change to ostringstream?
    path << outputDir << "/" << basename << ".h" << std::ends;
    char *pathStr = path.str();
    std::ofstream writer(pathStr);
    if (!writer.bad())
    {
        writer << "#pragma once" << std::endl;

        std::string baseClassName(basename);
        std::string baseFields;
        defineType(writer, 0, baseClassName, baseFields);

        for (auto &type : types)
        {
            std::stringstream ss(type);
            std::string className, fields;
            std::getline(ss, className, ':');
            std::getline(ss, fields, ':');
            trimString(className);
            trimString(fields);
            defineType(writer, basename, className, fields);
        }

        writer.close();
    }
    else
    {
        std::cerr << "Error opening " << path.str() << " for writing." << std::endl;
    }
    path.freeze(false);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: lox-generate-ast <output-directory>" << std::endl;
        return EXIT_USAGE;
    }

    std::vector<std::string> types = {
        "Binary   : Expr left, Token operator, Expr right",
        "Grouping : Expr expression",
        "Literal  : Object value",
        "Unary    : Token operator, Expr right"
    };
    const char *outputDir = argv[1];
    defineAst(outputDir, "Expr", types);

    return EXIT_SUCCESS;
}