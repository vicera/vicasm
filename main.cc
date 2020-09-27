#include <cstdio>

#include <iostream>
#include <string>

#include "parser.h"
#include "assembler.h"
#include "logging.h"

Logging lg = "main.cc";

int main(int argc, char** argv)
{
    if (argc <= 1)
        return 1;

    std::string fn = argv[1];
    ASMAssembler assembler(fn, 0);
    
    for (int i = 2; i < argc; ++i)
    {
        fn = argv[i];
        assembler.add_file(fn);
    }

    std::ustring pog = assembler.assemble();
    
    FILE *f = fopen("a.out", "w");
    for (std::ustring::iterator it = pog.begin(); it != pog.end(); ++it)
        fputc(*it, f);
    fclose(f);

    return 0;
}
