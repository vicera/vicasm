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
}
