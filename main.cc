#include <cstdio>

#include <iostream>
#include <string>

#include "parser.h"
#include "assembler.h"
#include "logging.h"

Logging lg = "main.cc";

int main(int argc, char** argv)
{
    if (argc != 2)
        return 1;

    std::string fn = argv[1];
    ASMFile poggers(fn);
    ASMAssembler assembler(fn, 0);
    std::string pog = assembler.assemble();

    for (std::string::iterator it = pog.begin(); it != pog.end(); ++it)
        printf("%02x ", (BYTE)(*it));
    puts("");
    return 0;
}
