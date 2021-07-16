#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>

#include "lexer.h"
#include "parser.h"
#include "evaluator.h"


std::string readFileBunny(const std::string &file)
{
    std::ifstream ifs(file.c_str(), std::ios::in |
                                    std::ios::binary |
                                    std::ios::ate);

    std::ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<char> bytes(fileSize);
    ifs.read(bytes.data(), fileSize);

    return std::string(bytes.data(), fileSize);
}


int main(int argc, char *argv[])
{



    //assert(argc > 1);

    std::string filename("test.bn");

    std::string input = readFileBunny(filename);


    bunny::lexer l(input);


    bunny::Parser p(l);

    bunny::Evaluator e;

    bunny::Environment *env = new bunny::Environment();

    bunny::Program *prg = p.parseProgram();



    if(p.getErrors().size())
    {
        std::cout << "Syntax Error: " << std::endl;

        for (auto error: p.getErrors())
            std::cout << error << std::endl;

        return 0;

    }

    //Crashes right there!
    bunny::Object *o = e.Eval(prg, env);

    return 0;
}
