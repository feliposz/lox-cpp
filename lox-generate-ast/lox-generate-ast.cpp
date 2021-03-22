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

void defineType(std::ofstream &writer, const char *basename, std::string &className, std::string &fields)
{
    writer << "template <typename R>" << std::endl;
    if (basename)
    {
        writer << "struct " << className << " : public " << basename << std::endl;
    }
    else
    {
        writer << "struct " << className << std::endl;
    }
        
    std::vector<std::string> fieldVec;
    splitString(',', fields, fieldVec);
    
    writer << "{" << std::endl;

    // Members
    for (auto &field : fieldVec)
    {
        std::vector<std::string> parts;
        splitString(' ', field, parts);
        writer << "    " << parts[0] << " *" << parts[1] << ";" << std::endl;
    }

    // Constructor parameters
    writer << std::endl;
    writer << "    " << className << "(";
    bool first = true;
    for (auto &field : fieldVec)
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
    writer << ")" << std::endl;

    // Constructor body
    writer << "    {" << std::endl;
    for (auto &field : fieldVec)
    {
        std::vector<std::string> parts;
        splitString(' ', field, parts);
        writer << "        this->" << parts[1] << " = " << parts[1] << ";" << std::endl;
    }
    writer << "    }" << std::endl;

    // Accept method
    if (basename)
    {
        writer << std::endl;
        writer << "    override accept(Visitor<R> visitor)" << std::endl;
        writer << "    {" << std::endl;
        writer << "        return visitor.visit" << className << basename << "(this);" << std::endl;
        writer << "    }" << std::endl;
    }
    else
    {
        writer << std::endl;
        writer << "    virtual R accept(Visitor<R> visitor);" << std::endl;
    }

    writer << "};" << std::endl << std::endl;
}

void defineVisitor(std::ofstream &writer, std::string &baseName, std::vector<std::string> &types)
{
    std::string lowerBaseName;
    for (auto c : baseName)
    {
        lowerBaseName.append(1, tolower(c));
    }

    // forward declarations
    for (auto &type : types)
    {
        std::vector<std::string> parts;
        splitString(':', type, parts);
        writer << "struct " << parts[0] << ";" << std::endl;
    }

    // "abstract" visitor
    writer << std::endl;
    writer << "template <typename R>" << std::endl;
    writer << "struct Visitor" << std::endl;
    writer << "{" << std::endl;
    for (auto &type : types)
    {
        std::vector<std::string> parts;
        splitString(':', type, parts);
        writer << "    R visit" << parts[0] << baseName << "(" << parts[0] << " *" << lowerBaseName << ");" << std::endl;
    }
    writer << "};" << std::endl << std::endl;
}

void defineAst(const char *outputDir, const char *basename, std::vector<std::string> &types)
{
    std::ostringstream path; // TODO: change to ostringstream?
    path << outputDir << "/" << basename << ".h" << std::ends;
    std::ofstream writer(path.str());
    if (!writer.bad())
    {
        writer << "#pragma once" << std::endl << std::endl;
        writer << "#include \"Token.h\"" << std::endl << std::endl;
        
        std::string baseClassName(basename);
        std::string baseFields;
        defineVisitor(writer, baseClassName, types);
        defineType(writer, 0, baseClassName, baseFields);

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

    std::vector<std::string> types = {
        "Binary   : Expr left, Token oper, Expr right",
        "Grouping : Expr expression",
        "Literal  : Object value",
        "Unary    : Token oper, Expr right"
    };
    const char *outputDir = argv[1];
    defineAst(outputDir, "Expr", types);

    return EXIT_SUCCESS;
}