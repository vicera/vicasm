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

typedef enum
{
    // halt
    IHALT,
    // nop
    INOP     = HALT      + 1,

    // push nn
    IPUSH_NN = NOP       + 1,
    // pusha
    IPUSHA   = PUSH_NN   + 3,
    // pop nn
    IPOP_NN  = PUSHA     + 1,
    // popa
    IPOPA    = POP_NN    + 3,

    // mov
    // mov r, r
    IMOV_RR      = POPA      + 1,
    // mov (HL), r
    IMOV_PR      = MOV_RR    + 49,
    // mov r, n
    IMOV_RN      = MOV_PR    + 7,
    // mov (HL), n
    IMOV_PN      = MOV_RN    + 7,
    // mov r, (HL)
    IMOV_RP      = MOV_PN    + 1,

    // mov SP, nn
    IMOV_SPNN    = MOV_RP    + 7,
    // mov SP, HL
    IMOV_SPHL    = MOV_SPNN  + 1,
    // mov HL, SP
    IMOV_HLSP    = MOV_SPHL  + 1,

`   // mov a, (BC/DE)
    IMOV_ARR     = MOV_HLSP  + 1,
    // mov a, (nn)
    IMOV_ANN     = MOV_ARR   + 2,
    // mov (BC/DE), a
    IMOV_RRA     = MOV_ANN   + 1,
    // mov (nn), a
    IMOV_NNA     = MOV_RRA   + 2,

    // ALU
    // add r
    IADD_R       = MOV_NNA   + 1,
    // add n
    IADD_N       = ADD_R     + 7,
    // add (hl)
    IADD_P       = ADD_N     + 1,
    
    // sub r
    ISUB_R       = ADD_P     + 1,
    // sub n
    ISUB_N       = SUB_R     + 7,
    // sub (hl)
    ISUB_P       = SUB_N     + 1,

    // and r
    IAND_R       = SUB_P     + 1,
    // and n
    IAND_N       = AND_R     + 7,
    // and (hl)
    IAND_P       = AND_N     + 1,
    
    // or r
    IOR_R        = AND_P    + 1,
    // or n
    IOR_N        = OR_R     + 7,
    // or (hl)
    IOR_P        = OR_N     + 1,

    // xor r
    IXOR_R       = OR_P      + 1,
    // xor n
    IXOR_N       = XOR_R     + 7,
    // xor (hl)
    IXOR_P       = XOR_N     + 1,

    // inc/dec
    // inc r/rr
    IINC_R       = XOR_P     + 1,
    IINC_RR      = INC_R     + 7,

    // dec r/rr
    IDEC_R       = INC_RR    + 3,
    IDEC_RR      = DEC_R     + 7,

    // sl/sr
    // sl r
    ISL_R        = DEC_RR    + 3,
    // sr r
    ISR_R        = SL_R      + 7,

    // cp r
    ICP_R        = SR_R      + 7,

    // Jumps
    // jp nn/(hl)
    IJP_NN       = CP_R      + 7,
    IJP_P        = JP_NN     + 1,

    // jc nn/(hl)
    IJC_NN,
    IJC_P,

    // jz nn/(hl)
    IJZ_NN,
    IJZ_P,

    // jn nn/(hl)
    IJN_NN,
    IJN_P,

    // call nn/(hl)
    ICALL_NN,
    ICALL_P,

    // ret
    IRET,
    
    // TODO: Finish that

} Opcodes;

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
