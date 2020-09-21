/*
 * VICasm Assembler by h34ting4ppliance
 *
 * assembler.h
 *
 * Assembler headers.
 */
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <vector>
#include "parser.h"

// Assembler class
class ASMAssembler
{
    private:
        // Main file
        std::string             mainfile;
        // Files
        std::vector<ASMFile>    files;
    public:
        // Init function.
        ASMAssembler    (std::string);
        // Add a file into the class.
        void            add_file(std::string);
        // Assemble and insert the binary into an array of characters
        char*           assemble();
};

#endif
