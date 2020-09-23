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

    lg.log("This is a test file.");
    lg.warn("!! I have warned you !!");
    lg.error("O gee random error %s %d", "oofie", 42);

    size_t count = 0;
    for (size_t i = 0; i < poggers.lsize; ++i)
        // The match function will filter a command to check
        // if it matches the command, number of arguments and
        // argument types.
        //
        // For example, this program counts the number mov commands
        // that has 2 8-bit registers.
        count += poggers.commands[i].match(MOV, 2, BIT8_REG, BIT8_REG);

    printf("%ld\n", count);
    return 0;
}
