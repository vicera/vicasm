#include <cstdio>

#include <iostream>
#include <string>

#include "parser.h"

#define MOV_RP MOV, 2, BIT8_REG, REG_POINTER

int main(int argc, char** argv)
{
    if (argc != 2)
        return 1;

    std::string fn = argv[1];
    ASMFile poggers(fn);

    size_t count = 0;
    for (size_t i = 0; i < poggers.lsize; ++i)
        count += poggers.commands[i].match(MOV_RP);

    printf("%ld\n", count);
    return 0;
}
