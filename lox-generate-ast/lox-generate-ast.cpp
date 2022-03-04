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
    writer << "    " << className << "(" << className << "Type type) : type(type) {}" << std::endl;;
    writer << "    virtual ~" << className << "() {}" << std::endl;
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
    writer << ") : " << basename << "(" << basename << "Type_" << className << ")";

    // Constructor body
    for (auto &field : fields)
    {
        std::vector<std::string> parts;
        splitString(' ', field, parts);
        writer << ", " << parts[1] << "(" << parts[1] << ")";
    }
    writer << " {}" << std::endl;

    // Destructor
    writer << std::endl;
    writer << "    ~" << className << "();" << std::endl;

    writer << "};" << std::endl << std::endl;
}

void implementType(std::ofstream &writer, const char *basename, std::string &className, std::string &fieldStr)
{
    std::vector<std::string> fields;

    splitString(',', fieldStr, fields);

    // Destructor
    writer << std::endl;
    writer << className << "::~" << className << "()" << std::endl;
    writer << "{" << std::endl;
    for (auto &field : fields)
    {
        std::vector<std::string> parts;
        splitString(' ', field, parts);
        writer << "    delete " << parts[1] << ";" << std::endl;
    }
    writer << "}" << std::endl;
}

void defineAst(const char *outputDir, const char *basename, std::vector<std::string> &types, std::vector<std::string> &includes)
{
    std::ostringstream pathH, pathCpp; // TODO: change to ostringstream?
    pathH << outputDir << "/" << basename << ".h" << std::ends;
    pathCpp << outputDir << "/" << basename << ".cpp" << std::ends;
    std::ofstream writerH(pathH.str());
    std::ofstream writerCpp(pathCpp.str());
    if (writerH.bad())
    {
        std::cerr << "Error opening " << pathH.str() << " for writing." << std::endl;
    }
    else if (writerCpp.bad())
    {
        std::cerr << "Error opening " << pathCpp.str() << " for writing." << std::endl;
    }
    else
    {
        writerH << "#pragma once" << std::endl << std::endl;

        for (auto &include : includes)
        {
            writerH << "#include \"" << include << ".h\"" << std::endl;
        }

        writerCpp << "#include \"" << basename << ".h\"" << std::endl;
        if (strcmp(basename, "Expr") == 0)
        {
            writerH << "struct Block;" << std::endl;
            writerCpp << "#include \"Stmt.h\"" << std::endl;
        }

        writerH << std::endl;
        writerH << "enum " << basename << "Type" << std::endl;
        writerH << "{" << std::endl;
        for (auto &type : types)
        {
            std::vector<std::string> parts;
            splitString(':', type, parts);
            writerH << "    " << basename << "Type_" << parts[0] << "," << std::endl;
        }
        writerH << "};" << std::endl << std::endl;

        std::string baseClassName(basename);
        defineBaseType(writerH, baseClassName);

        for (auto &type : types)
        {
            std::vector<std::string> parts;
            splitString(':', type, parts);
            defineType(writerH, basename, parts[0], parts[1]);
            implementType(writerCpp, basename, parts[0], parts[1]);
        }

        writerH.close();
        writerCpp.close();
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
        "Set      : Expr object, Token name, Expr value",
        "Super    : Token keyword, Token method",
        "This     : Token keyword",
        "Unary    : Token oper, Expr right",
        "Variable : Token name",
        "Call     : Expr callee, Token paren, ListExpr arguments",
        "Get      : Expr object, Token name",
        "Lambda   : Token keyword, ListToken params, Block body",
    };
    std::vector<std::string> exprIncludes = { "Token", "ListToken", "ListExpr" };
    defineAst(outputDir, "Expr", exprTypes, exprIncludes);

    std::vector<std::string> stmtTypes = {
        "Expression : Expr expression",
        "Print      : Expr expression",
        "Var        : Token name, Expr initializer",
        "Block      : ListStmt statements",
        "Function   : Token name, ListToken params, Block body",
        "Class      : Token name, Variable superclass, ListFunction methods, ListFunction statics",
        "If         : Expr condition, Stmt thenBranch, Stmt elseBranch",
        "While      : Expr condition, Stmt body",
        "Return     : Token keyword, Expr value",
        "Break      : Token keyword",
    };
    std::vector<std::string> stmtIncludes = { "Expr", "ListStmt", "ListToken", "ListFunction" };
    defineAst(outputDir, "Stmt", stmtTypes, stmtIncludes);

    return EXIT_SUCCESS;
}