/*
 * VICasm Assembler by h34ting4ppliance
 *
 * assembler.cc
 *
 * Contains functions to assemble a bunch of ASM files.
 */
#include <cstdio>
#include <cstdlib>

#include <string>
#include <vector>

#include "parser.h"

/*
 * ASMAssembler File Adding
 *
 * Appends a file to the files vector.
 */
void ASMAssembler::add_file(std::string fn)
{
    ASMFile f = fn;
    files.push_back(f);
}

/*
 * ASMAssembler Init Function
 * std::string -> ASMAssembler
 *
 * Function that runs on class definition.
 * Main file must be specified.
 */
ASMAssembler::ASMAssembler (std::string mf)
{
    // Assembling a file
    ASMFile f = mf;
    // Appending it to the files vector.
    files.push_back(f);
    // Setting it as the main file
    mainfile = mf;
}
