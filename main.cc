#include <cstdio>

#include <iostream>
#include <string>

#include "parser.h"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

int main(int argc, char** argv)
{
    if (argc != 2)
        return 1;

    std::string fn = argv[1];
    ASMFile poggers(fn);

    for (size_t i = 0; i < poggers.lsize; ++i)
    {
        printf("Line %3ld: CMD %02x ",
                poggers.commands[i].linenum,
                poggers.commands[i].cmd);
        for (size_t j = 0; j < poggers.commands[i].arsize; ++j)
        {
            printf("- %02x ", poggers.commands[i].args[j].argtype);
            if (poggers.commands[i].args[j].argtype == STRING ||
                poggers.commands[i].args[j].argtype == DEF_POINTER ||
                poggers.commands[i].args[j].argtype == DEF_CONST)
                printf("%s ", poggers.commands[i].args[j].content.s);
            else
                printf("%04x ", poggers.commands[i].args[j].content.i);
        }
        putchar('\n');
    }
}
