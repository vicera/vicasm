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
#include <map>
#include "parser.h"



// To check if the parsed command
// matches the opcode
#define SHALT       HALT,   0
#define SNOP        NOP,    0

// stack
#define SPUSH_RR    PUSH,   1,  BIT16_REG
#define SPUSHA      PUSHA,  0
#define SPOP_RR     POP,    1,  BIT16_REG
#define SPOPA       POPA,   0

// mov
#define SMOV_RR     MOV,    2,  BIT8_REG,       BIT8_REG
#define SMOV_PR     MOV,    2,  REG_POINTER,    BIT8_REG
#define SMOV_RN     MOV,    2,  BIT8_REG,       NUM_CONST
#define SMOV_RD     MOV,    2,  BIT8_REG,       DEF_CONST

#define SMOV_PN     MOV,    2,  REG_POINTER,    NUM_CONST
#define SMOV_PD     MOV,    2,  REG_POINTER,    DEF_CONST
#define SMOV_RP     MOV,    2,  BIT8_REG,       REG_POINTER

#define SMOV_SPNN   MOV,    2,  SP_REG,         NUM_CONST
#define SMOV_SPDD   MOV,    2,  SP_REG,         DEF_CONST
#define SMOV_SPHL   MOV,    2,  SP_REG,         BIT16_REG
#define SMOV_HLSP   MOV,    2,  BIT16_REG,      SP_REG

#define SMOV_ARR    MOV,    2,  BIT8_REG,       REG_POINTER
#define SMOV_ANN    MOV,    2,  BIT8_REG,       NUM_POINTER
#define SMOV_ADD    MOV,    2,  BIT8_REG,       DEF_POINTER
#define SMOV_RRA    MOV,    2,  REG_POINTER,    BIT8_REG
#define SMOV_NNA    MOV,    2,  NUM_POINTER,    BIT8_REG
#define SMOV_DDA    MOV,    2,  DEF_POINTER,    BIT8_REG

#define SMOV_RRNN   MOV,    2,  BIT16_REG,      NUM_CONST
#define SMOV_RRDD   MOV,    2,  BIT16_REG,      DEF_CONST

// add
#define SADD_R      ADD,    1,  BIT8_REG
#define SADD_N      ADD,    1,  NUM_CONST
#define SADD_D      ADD,    1,  DEF_CONST
#define SADD_P      ADD,    1,  REG_POINTER

// sub
#define SSUB_R      SUB,    1,  BIT8_REG
#define SSUB_N      SUB,    1,  NUM_CONST
#define SSUB_D      SUB,    1,  DEF_CONST
#define SSUB_P      SUB,    1,  REG_POINTER

// and
#define SAND_R      AND,    1,  BIT8_REG
#define SAND_N      AND,    1,  NUM_CONST
#define SAND_D      AND,    1,  DEF_CONST
#define SAND_P      AND,    1,  REG_POINTER

// or
#define SOR_R       OR,     1,  BIT8_REG
#define SOR_N       OR,     1,  NUM_CONST
#define SOR_D       OR,     1,  DEF_CONST
#define SOR_P       OR,     1,  REG_POINTER

// xor
#define SXOR_R      XOR,    1,  BIT8_REG
#define SXOR_N      XOR,    1,  NUM_CONST
#define SXOR_D      XOR,    1,  DEF_CONST
#define SXOR_P      XOR,    1,  REG_POINTER

// inc/dec
// inc r/rr
#define SINC_R      INC,    1,  BIT8_REG
#define SINC_RR     INC,    1,  BIT16_REG
// dec r/rr
#define SDEC_R      DEC,    1,  BIT8_REG
#define SDEC_RR     DEC,    1,  BIT16_REG

// sl/sr
// sl
#define SSL_R       SL,     1,  BIT8_REG
// sr
#define SSR_R       SR,     1,  BIT8_REG

// cp r
#define SCP_R       CP,     1,  BIT8_REG

// jp
// jp nn
#define SJP_NN      JP,     1,  NUM_CONST
#define SJP_DD      JP,     1,  DEF_CONST
// jp HL
#define SJP_P       JP,     1,  BIT18_REG

// jc
// jc nn
#define SJC_NN      JC,     1,  NUM_CONST
#define SJC_DD      JC,     1,  DEF_CONST
// jc HL
#define SJC_P       JC,     1,  BIT16_REG

// jz
// jz nn
#define SJZ_NN      JZ,     1,  NUM_CONST
#define SJZ_DD      JZ,     1,  DEF_CONST
// jz HL
#define SJZ_P       JZ,     1,  BIT16_REG

// jn
// jn nn
#define SJN_NN      JN,     1,  NUM_CONST
#define SJN_DD      JN,     1,  DEF_CONST
// jn HL
#define SJN_P       JN,     1,  BIT16_REG

// call
// call nn
#define SCALL_NN    CALL,   1,  NUM_CONST
#define SCALL_DD    CALL,   1,  DEF_CONST
// call HL
#define SCALL_P     CALL,   1,  BIT16_REG

// ret
#define SRET        RET,    0

// dumpr/dumpm
#define SDUMP_R     DUMP_R, 0
#define SDUMP_M     DUMP_M, 1,  NUM_CONST
#define SDUMP_MD    DUMP_M, 1,  DEF_CONST

// slp
#define SSLP        SLP,    0

// swap
#define SSWAP_R     SWAP,   1,  BIT8_REG
#define SSWAP_RR    SWAP,   1,  BIT16_REG

// .define
#define SDEFINE     DEFINE, 2,  STRING, NUM_CONST
// .include
#define SINCLUDE    INCLUDE,1,  STRING
// .bin
#define SBIN        BIN,    1,  STRING
// .org
#define SORG        ORG,    1,  NUM_CONST
// Label
#define SLABEL      LBL,    1,  STRING

//////////////////////////

// Opcodes enum
typedef enum
{
    // halt
    IHALT,
    // nop
    INOP     = IHALT      + 1,

    // push nn
    IPUSH_RR = INOP       + 1,
    // pusha
    IPUSHA   = IPUSH_RR   + 3,
    // pop nn
    IPOP_RR  = IPUSHA     + 1,
    // popa
    IPOPA    = IPOP_RR    + 3,

    // mov
    // mov r, r
    IMOV_RR      = IPOPA      + 1,
    // mov (HL), r
    IMOV_PR      = IMOV_RR    + 49,
    // mov r, n
    IMOV_RN      = IMOV_PR    + 7,
    // mov (HL), n
    IMOV_PN      = IMOV_RN    + 7,
    // mov r, (HL)
    IMOV_RP      = IMOV_PN    + 1,

    // mov SP, nn
    IMOV_SPNN    = IMOV_RP    + 7,
    // mov SP, HL
    IMOV_SPHL    = IMOV_SPNN  + 1,
    // mov HL, SP
    IMOV_HLSP    = IMOV_SPHL  + 1,

    // mov a, (BC/DE)
    IMOV_ARR     = IMOV_HLSP  + 1,
    // mov a, (nn)
    IMOV_ANN     = IMOV_ARR   + 2,
    // mov (BC/DE), a
    IMOV_RRA     = IMOV_ANN   + 1,
    // mov (nn), a
    IMOV_NNA     = IMOV_RRA   + 2,
    
    // mov rr, nn
    IMOV_RRNN    = IMOV_NNA   + 1,

    // ALU
    // add r
    IADD_R       = IMOV_NNA   + 3,
    // add n
    IADD_N       = IADD_R     + 7,
    // add (hl)
    IADD_P       = IADD_N     + 1,
    
    // sub r
    ISUB_R       = IADD_P     + 1,
    // sub n
    ISUB_N       = ISUB_R     + 7,
    // sub (hl)
    ISUB_P       = ISUB_N     + 1,

    // and r
    IAND_R       = ISUB_P     + 1,
    // and n
    IAND_N       = IAND_R     + 7,
    // and (hl)
    IAND_P       = IAND_N     + 1,
    
    // or r
    IOR_R        = IAND_P    + 1,
    // or n
    IOR_N        = IOR_R     + 7,
    // or (hl)
    IOR_P        = IOR_N     + 1,

    // xor r
    IXOR_R       = IOR_P      + 1,
    // xor n
    IXOR_N       = IXOR_R     + 7,
    // xor (hl)
    IXOR_P       = IXOR_N     + 1,

    // inc/dec
    // inc r/rr
    IINC_R       = IXOR_P     + 1,
    IINC_RR      = IINC_R     + 7,

    // dec r/rr
    IDEC_R       = IINC_RR    + 3,
    IDEC_RR      = IDEC_R     + 7,

    // sl/sr
    // sl r
    ISL_R        = IDEC_RR    + 3,
    // sr r
    ISR_R        = ISL_R      + 7,

    // cp r
    ICP_R        = ISR_R      + 7,

    // Jumps
    // jp nn/(hl)
    IJP_NN       = ICP_R      + 7,
    IJP_P        = IJP_NN     + 1,

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
 
    // dumpr/dumpm
    IDUMP_R,
    IDUMP_M,

    // slp
    ISLP,

    // swap A, r
    ISWAP_R      = ISLP       + 1,
    // swap HL, rr
    ISWAP_RR     = ISWAP_R     + 7
} Opcodes;

// Assembler Label/Definition class
class ASMLabel
{
    private:
        std::string name;
        WORD value;
    public:
        ASMLabel (std::string, WORD, bool);
        void insert_def(std::string&, size_t);
        // This value may be modified later.
        bool is_word;
};

// Assembler class
class ASMAssembler
{
    private:
        // Actual assemble function
        void            assemble_one(const char*, std::string&);
        // Binary appending function
        void            append_binary(ASMBinary&);
        // Append label loc
        void            append_label_loc(std::string, WORD, bool);

        // Labels
        std::map<std::string, WORD> labels;
        // Labels location
        std::vector<ASMLabel>   labels_loc;

        // Location pointer
        WORD                    locpointer;
        // Main file
        std::string             mainfile;
        // Files
        std::vector<ASMFile>    files;
    public:
        // Init function.
        ASMAssembler    (std::string, WORD);
        // Add a file into the class.
        void            add_file(std::string);
        // Assemble and insert the binary into an array of characters
        std::string     assemble();
};

#endif
