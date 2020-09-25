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
#include <map>

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
 * ASMLabel Init Function
 */
ASMLabel::ASMLabel (std::string a_name, WORD a_value, bool a_isword = false)
{
    name    = a_name;
    value   = a_value;
    is_word = a_isword;
}

/*
 * Insert a definition into a string
 */
void ASMLabel::insert_def(std::string& a_str, size_t where)
{
    if (is_word)
    {
        a_str[where]    = value / 0x100;
        a_str[where+1]  = value % 0x100;
    }
    else
        a_str[where]    = value % 0x100;
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
 * LABEL Functions
 * 
 * The assembler has 2 Label vectors: The location one and the labels one
 *
 * The location one contains all the location where empty bytes need to be
 * replaced by constant values. The other labels contains these values.
 *
 * Basically, the assembler has two phases, one that assembles the program,
 * another one that fills with constant definitions.
 */
void ASMAssembler::append_label_loc(std::string name, WORD location, bool size)
{
    ASMLabel lbl(name, location, size);
    labels_loc.push_back(lbl);
}

// H - 0xXX00 - false
// L = 0x00XX - true
inline char sepchars(WORD in, bool hl)
{
    return hl ? in % 0x100 : in / 0x100;
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
    #define APP8(x)     (char)(sepchars(ARG(x).content.i, true))
    #define APP16(x)    (char)(sepchars(ARG(x).content.i, false)), \
                        (char)(sepchars(ARG(x).content.i, true))
    #define GREG8(x)    (ARG(x).content.i - A)
    #define GREG16(x)   (ARG(x).content.i - HL)
    #define GSTR(x)     ARG(x).content.s
    #define IS_HL(x)    (ARG(x).content.i == HL)
    #define IS_A(x)     (ARG(x).content.i == A)
    #define UPDATE_LC() locpointer = prgm.size()
    #define ALBL(x,y)       labels[x] = y
    #define ALBLOC(x,z)     append_label_loc(x, (UPDATE_LC()), z); \
                            prgm += '\0'; if (z) prgm += '\0'

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

        // halt and nop
        if      (MATCH(SHALT))
            prgm += IHALT;
        else if (MATCH(SNOP))
            prgm += INOP;

        // stack ops
        else if (MATCH(SPUSH_RR))
            prgm += IPUSH_RR + GREG16(0);
        else if (MATCH(SPUSHA))
            prgm += IPUSHA;
        else if (MATCH(SPOP_RR))
            prgm += IPOP_RR + GREG16(0);
        else if (MATCH(SPOPA))
            prgm += IPOPA;
        
        // mov r/(hl), r/n
        else if (MATCH(SMOV_RR))
            prgm += IMOV_RR + (7 * GREG8(0) + GREG8(1));
        else if (MATCH(SMOV_PR) && IS_HL(0))
            prgm += IMOV_PR + GREG8(1);
        else if (MATCH(SMOV_RN))
            prgm += {IMOV_RN + GREG8(0), APP8(1)};
        else if (MATCH(SMOV_RD))
        {
            prgm += IMOV_RN + GREG8(0);
            ALBLOC(GSTR(1), false);
        }
        
        // mov r/(hl), n/(hl)
        else if (MATCH(SMOV_PN) && IS_HL(0))
            prgm += {IMOV_PN, APP8(1)};
        else if (MATCH(SMOV_PD) && IS_HL(0))
        {
            prgm += IMOV_PN;
            ALBLOC(GSTR(1), false);
        }
        else if (MATCH(SMOV_RP) && IS_HL(1))
            prgm += IMOV_RP + GREG8(0);

        // mov HL/SP, HL/nn
        else if (MATCH(SMOV_SPNN))
            prgm += {IMOV_SPNN, APP16(1)};
        else if (MATCH(SMOV_SPDD))
        {
            prgm += IMOV_SPNN;
            ALBLOC(GSTR(1), true);
        }
        else if (MATCH(SMOV_SPHL) && IS_HL(1))
            prgm += IMOV_SPHL;
        else if (MATCH(SMOV_HLSP) && IS_HL(0))
            prgm += IMOV_HLSP;
        
        // mov A/rr, A/rr/nn
        else if (MATCH(SMOV_ARR) && IS_A(0))
            prgm += IMOV_ARR + GREG16(1) - 1;
        else if (MATCH(SMOV_ANN) && IS_A(0))
            prgm += {IMOV_ANN, APP16(1)};
        else if (MATCH(SMOV_ADD) && IS_A(0))
        {
            prgm += IMOV_ANN;
            ALBLOC(GSTR(1), true);
        }
        
        else if (MATCH(SMOV_RRA) && IS_A(1))
            prgm += IMOV_RRA + GREG16(0) - 1;
        else if (MATCH(SMOV_NNA) && IS_A(1))
            prgm += {IMOV_NNA, APP16(0)};
        else if (MATCH(SMOV_DDA) && IS_A(1))
        {
            prgm += IMOV_NNA;
            ALBLOC(GSTR(0), true);
        }

        // mov rr, nn
        else if (MATCH(SMOV_RRNN))
            prgm += {IMOV_RRNN + GREG16(0), APP16(1)};
        else if (MATCH(SMOV_RRDD))
        {
            prgm += IMOV_RRNN + GREG16(0);
            ALBLOC(GSTR(0), true);
        }

        // add
        else if (MATCH(SADD_R))
            prgm += IADD_R + GREG8(0);
        else if (MATCH(SADD_N))
            prgm += {IADD_N, APP8(0)};
        else if (MATCH(SADD_D))
        {
            prgm += IADD_N;
            ALBLOC(GSTR(0), false);
        }
        else if (MATCH(SADD_P) && IS_HL(0))
            prgm += IADD_P;

        // sub
        else if (MATCH(SSUB_R))
            prgm += ISUB_R + GREG8(0);
        else if (MATCH(SSUB_N))
            prgm += {ISUB_N, APP8(0)};
        else if (MATCH(SSUB_D))
        {
            prgm += ISUB_N;
            ALBLOC(GSTR(0), false);
        }
        else if (MATCH(SSUB_P) && IS_HL(0))
            prgm += ISUB_P;

        // and
        else if (MATCH(SAND_R))
            prgm += IAND_R + GREG8(0);
        else if (MATCH(SAND_N))
            prgm += {IAND_N, APP8(0)};
        else if (MATCH(SAND_D))
        {
            prgm += IAND_N;
            ALBLOC(GSTR(0), false);
        }
        else if (MATCH(SAND_P) && IS_HL(0))
            prgm += IAND_P;

        // or
        else if (MATCH(SOR_R))
            prgm += IOR_R + GREG8(0);
        else if (MATCH(SOR_N))
            prgm += {IOR_N, APP8(0)};
        else if (MATCH(SOR_D))
        {
            prgm += IOR_N;
            ALBLOC(GSTR(0), false);
        }
        else if (MATCH(SOR_P) && IS_HL(0))
            prgm += IOR_P;

        // xor
        else if (MATCH(SXOR_R))
            prgm += IXOR_R + GREG8(0);
        else if (MATCH(SXOR_N))
            prgm += {IXOR_N, APP8(0)};
        else if (MATCH(SXOR_D))
        {
            prgm += IXOR_N;
            ALBLOC(GSTR(0), false);
        }
        else if (MATCH(SXOR_P) && IS_HL(0))
            prgm += IXOR_P;
        
        // inc
        else if (MATCH(SINC_R))
            prgm += IINC_R + GREG8(0);
        else if (MATCH(SINC_RR))
            prgm += IINC_RR + GREG16(0);
        // dec
        else if (MATCH(SDEC_R))
            prgm += IDEC_R + GREG8(0);
        else if (MATCH(SDEC_RR))
            prgm += IDEC_RR + GREG16(0);

        // sl
        else if (MATCH(SSL_R))
            prgm += ISL_R + GREG8(0);
        // sr
        else if (MATCH(SSR_R))
            prgm += ISR_R + GREG8(0);

        // jp
        else if (MATCH(SJP_NN))
            prgm += {IJP_NN, APP16(0)};
        else if (MATCH(SJP_DD))
        {
            prgm += IJP_NN;
            ALBLOC(GSTR(0), true);
        }
        else if (MATCH(SJP_P) && IS_HL(0))
            prgm += IJP_P;

        // jc
        else if (MATCH(SJC_NN))
            prgm += {IJC_NN, APP16(0)};
        else if (MATCH(SJC_DD))
        {
            prgm += IJC_NN;
            ALBLOC(GSTR(0), true);
        }
        else if (MATCH(SJC_P) && IS_HL(0))
            prgm += IJC_P;

        // jz
        else if (MATCH(SJZ_NN))
            prgm += {IJZ_NN, APP16(0)};
        else if (MATCH(SJZ_DD))
        {
            prgm += IJZ_NN;
            ALBLOC(GSTR(0), true);
        }
        else if (MATCH(SJZ_P) && IS_HL(0))
            prgm += IJZ_P;

        // jn
        else if (MATCH(SJN_NN))
            prgm += {IJN_NN, APP16(0)};
        else if (MATCH(SJN_DD))
        {
            prgm += IJN_NN;
            ALBLOC(GSTR(0), true);
        }
        else if (MATCH(SJN_P) && IS_HL(0))
            prgm += IJN_P;

        // call
        else if (MATCH(SCALL_NN))
            prgm += {ICALL_NN, APP16(0)};
        else if (MATCH(SCALL_DD))
        {
            prgm += ICALL_NN;
            ALBLOC(GSTR(0), true);
        }
        else if (MATCH(SCALL_P) && IS_HL(0))
            prgm += ICALL_P;

        // ret
        else if (MATCH(SRET))
            prgm += IRET;
        
        // dumpr
        else if (MATCH(SDUMP_R))
            prgm += IDUMP_R;
        // dumpm
        else if (MATCH(SDUMP_M))
            prgm += {IDUMP_M, APP16(0)};
        else if (MATCH(SDUMP_MD))
        {
            prgm += IDUMP_M;
            ALBLOC(GSTR(0), true);
        }

        // slp
        else if (MATCH(SSLP))
            prgm += ISLP;

        // swap
        else if (MATCH(SSWAP_R))
            prgm += ISWAP_R + GREG8(0);
        else if (MATCH(SSWAP_RR))
            prgm += ISWAP_RR + GREG16(0);

        /////////////////////////////
        // Assembler-wide commands //
        /////////////////////////////
        
        else if (MATCH(SDEFINE))
            labels[GSTR(0)] = APP16(0);
        else if (MATCH(SINCLUDE))
            assemble_one(GSTR(0), prgm);
        else if (MATCH(SBIN))
            ;
        else if (MATCH(SORG))
            ;
        else if (MATCH(SLABEL))
            labels[GSTR(0)] = (WORD)locpointer;

        else
            ldie("Invalid arguments", curcmd->linenum);
        
        UPDATE_LC();
        // TODO: Finish this block.
        // TODO: Fix the unsigned char issue.
    }
}

/*
 * ASMAssembler assemble function
 * void -> char*
 *
 * Assemble the program
 */
std::string ASMAssembler::assemble()
{
    std::string result = "";
    
    assemble_one(mainfile.c_str(), result);
    return result;
}
