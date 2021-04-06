#include <iostream>
#include <strstream>
#include <fstream>
#include <sstream>
#include <vector>

#define EXIT_USAGE 64

void trimString(std::string &str)
{
    str.erase(0, str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);
}

void splitString(char splitChar, std::string &str, std::vector<std::string> &splitStr)
{
    std::stringstream ss(str);
    std::string part;
    while (std::getline(ss, part, splitChar))
    {
        trimString(part);
        splitStr.push_back(part);
    }
}

void defineBaseType(std::ofstream &writer, std::string &className)
{
    writer << "struct " << className << std::endl;
    writer << "{" << std::endl;
    writer << "    " << className << "Type type;" << std::endl;
    writer << std::endl;
    writer << "    " << className << "(" << className << "Type type)" << std::endl;;
    writer << "    {" << std::endl;
    writer << "        this->type = type;" << std::endl;
    writer << "    }" << std::endl;
    writer << std::endl;
    writer << "    virtual ~" << className << "()" << std::endl;;
    writer << "    {" << std::endl;
    writer << "    }" << std::endl;
    writer << "};" << std::endl << std::endl;
}

void defineType(std::ofstream &writer, const char *basename, std::string &className, std::string &fieldStr)
{
    std::vector<std::string> fields;

    writer << "struct " << className << " : public " << basename << std::endl;
    writer << "{" << std::endl;

    splitString(',', fieldStr, fields);

    // Members
    for (auto &field : fields)
    {
        std::vector<std::string> parts;
        splitString(' ', field, parts);
        writer << "    " << parts[0] << " *" << parts[1] << ";" << std::endl;
    }

    // Constructor parameters
    writer << std::endl;
    writer << "    " << className << "(";
    bool first = true;
    for (auto &field : fields)
    {
        std::vector<std::string> parts;
        splitString(' ', field, parts);
        if (!first)
        {
            writer << ", ";
        }
        writer << parts[0] << " *" << parts[1];;
        first = false;
    }
    writer << ") : " << basename << "(" << basename << "Type_" << className << ")" << std::endl;

    // Constructor body
    writer << "    {" << std::endl;
    for (auto &field : fields)
    {
        std::vector<std::string> parts;
        splitString(' ', field, parts);
        writer << "        this->" << parts[1] << " = " << parts[1] << ";" << std::endl;
    }
    writer << "    }" << std::endl;

    // Destructor
    writer << std::endl;
    writer << "    ~" << className << "()" << std::endl;
    writer << "    {" << std::endl;
    for (auto &field : fields)
    {
        std::vector<std::string> parts;
        splitString(' ', field, parts);
        writer << "        delete " << parts[1] << ";" << std::endl;
    }
    writer << "    }" << std::endl;

    writer << "};" << std::endl << std::endl;
}

void defineAst(const char *outputDir, const char *basename, std::vector<std::string> &types, std::vector<std::string> &includes)
{
    std::ostringstream path; // TODO: change to ostringstream?
    path << outputDir << "/" << basename << ".h" << std::ends;
    std::ofstream writer(path.str());
    if (!writer.bad())
    {
        writer << "#pragma once" << std::endl << std::endl;

        for (auto &include : includes)
        {
            writer << "#include \"" << include << ".h\"" << std::endl;
        }

        writer << std::endl;
        writer << "enum " << basename << "Type" << std::endl;
        writer << "{" << std::endl;
        for (auto &type : types)
        {
            std::vector<std::string> parts;
            splitString(':', type, parts);
            writer << "    " << basename << "Type_" << parts[0] << "," << std::endl;
        }
        writer << "};" << std::endl << std::endl;

        std::string baseClassName(basename);
        defineBaseType(writer, baseClassName);

        for (auto &type : types)
        {
            std::vector<std::string> parts;
            splitString(':', type, parts);
            defineType(writer, basename, parts[0], parts[1]);
        }

        writer.close();
    }
    else
    {
        std::cerr << "Error opening " << path.str() << " for writing." << std::endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: lox-generate-ast <output-directory>" << std::endl;
        return EXIT_USAGE;
    }

    const char *outputDir = argv[1];

    std::vector<std::string> exprTypes = {
        "Assign   : Token name, Expr value",
        "Ternary  : Expr first, Token oper1, Expr second, Token oper2, Expr third",
        "Binary   : Expr left, Token oper, Expr right",
        "Logical  : Expr left, Token oper, Expr right",
        "Grouping : Expr expression",
        "Literal  : Object value",
        "Unary    : Token oper, Expr right",
        "Variable : Token name",
    };
    std::vector<std::string> exprIncludes = { "Token" };
    defineAst(outputDir, "Expr", exprTypes, exprIncludes);

    std::vector<std::string> stmtTypes = {
        "Expression : Expr expression",
        "Print : Expr expression",
        "Var : Token name, Expr initializer",
        "Block : ListStmt statements",
        "If : Expr condition, Stmt thenBranch, Stmt elseBranch",
        "While : Expr condition, Stmt body",
        "Break : ",
    };
    std::vector<std::string> stmtIncludes = { "Expr", "ListStmt" };
    defineAst(outputDir, "Stmt", stmtTypes, stmtIncludes);

    return EXIT_SUCCESS;
}