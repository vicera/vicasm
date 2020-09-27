#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include <iostream>
#include <string>

#include "parser.h"
#include "assembler.h"
#include "logging.h"

Logging lg = "main.cc";

void print_usage(const char* pname)
{
    printf("Usage: %s [-o OUTPUT] FILES...\n", pname);
}

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        print_usage(argv[0]);
        return 1;
    }

    std::string outfile = "a.out";
    std::string fn;
    
    int c;
    while ((c=getopt(argc, argv, "o:hu")) != EOF)
    {
        switch (c)
        {
            case 'o':
                outfile = optarg;
                break;
            case 'h':
            case 'u':
                print_usage(argv[0]);
                return 0;
                break;
            case '?':
            default:
                print_usage(argv[0]);
                return 1;
                break;
        }
    }
    if (optind == argc)
    {
        print_usage(argv[0]);
        return 1;
    }

    fn = argv[optind++];
    ASMAssembler assembler(fn, 0);

    for (int i = optind; i < argc; ++i)
    {
        fn = argv[i];
        assembler.add_file(fn);
    }
    std::ustring pog = assembler.assemble();
    
    FILE *f = fopen(outfile.c_str(), "w");
    if (!f)
    {
        perror("Unable to open file");
        exit(1);
    }
    for (std::ustring::iterator it = pog.begin(); it != pog.end(); ++it)
        fputc(*it, f);
    fclose(f);

    return 0;
}
