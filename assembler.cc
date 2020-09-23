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
#include "logging.h"
#include "assembler.h"

static Logging lg = "assembler.cc";

static void die(const char* message)
{
    lg.error("%s", message);
    exit(1);
}

static void ldie(const char* message, size_t line)
{
    if (line > 0)
        lg.error("Stop at line %ld", line);
    lg.error("%s", message);
    exit(1);
}

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
ASMAssembler::ASMAssembler (std::string mf, WORD sl = 0)
{
    // Assembling a file
    ASMFile f = mf;
    // Appending it to the files vector.
    files.push_back(f);
    // Setting it as the main file
    mainfile = mf;
    // Setting up the location pointer
    locpointer = sl;
}

/*
 * ASMAssemble bin appending function
 *
 * ASMBinary -> void
 *
 * Appends binary into the assembly file.
 */
void ASMAssembler::append_binary(ASMBinary& bin)
{
    
}

/*
 * ASMAssembler actual assemble function
 *
 * char*, std::string& -> void
 *
 * Assemble a file. A different function has been implemented to
 * recursively assemble files (for .include and .bin)
 */
void ASMAssembler::assemble_one(const char* filename, std::string& prgm)
{
    #define MATCH       curcmd->match
    #define ARG(x)      curcmd->args[x]
    #define APP8(x)     prgm += ARG(x).content.i % 0x100
    #define APP16(x)    prgm += {ARG(x).content.i / 0x100, ARG(x).content.i % 0x100}
    #define GREG8(x)    ARG(x).content.i - A
    #define GREG16(x)   ARG(x).content.i - HL

    std::vector<ASMFile>::iterator curfile;
    ASMCommand *curcmd;
    for (curfile = files.begin(); curfile != files.end(); ++curfile)
        if (curfile->filename == filename)
            break;
    if (curfile == files.end())
        die("Assembly file not found.");
    
    // Time for a hella long if else block.
    for (size_t ln = 0; ln < curfile->lsize; ++ln)
    {
        curcmd = &(curfile->commands[ln]);
        
        if      (MATCH(SHALT))
            prgm += IHALT;
        else if (MATCH(SNOP))
            prgm += INOP;
        else if (MATCH(SPUSH_RR))
            prgm += IPUSH_RR + GREG16(0);
        else if (MATCH(SPUSHA))
            prgm += IPUSHA;
        else if (MATCH(SPOP_RR))
            prgm += IPOP_RR + GREG16(0);
        else
            ldie("Invalid arguments", curcmd->linenum);

        // TODO: Finish this block.
    }
}

/*
 * ASMAssembler assemble function
 * void -> char*
 *
 * Assemble the program
 */
const char* ASMAssembler::assemble()
{
    std::string result = "";
    
    assemble_one(mainfile.c_str(), result);
    return result.c_str();
}
