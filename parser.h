/*
 *  VICasm Assembler by h34ting4ppliance
 *
 *  parser.h
 *
 *  Parser headers.
 */
#ifndef PARSER_H
#define PARSER_H

#include <cstdint>
#include <string>

// Type definition
typedef uint16_t WORD;
typedef uint8_t  BYTE;

// Commands
typedef enum
{
    C_NONE,

    // CPU Instruction set.
    HALT, NOP,

    PUSH, PUSHA, POP, POPA,

    MOV,
    ADD, SUB, AND, OR, XOR,
    INC, DEC, SL, SR,

    CP,
    JP, JC, JZ, JN, CALL, RET,

    DUMP_R, DUMP_M,
    
    SLP,
    SWAP,

    // Assembler-wide commands.
    DEFINE,
    INCLUDE,
    BIN,
    ORG,
    DB,
    LBL
} Commands;

typedef enum
{
    A_NONE,

    // Registers
    BIT8_REG,
    BIT16_REG,
    SP_REG,

    // Pointers
    NUM_POINTER,
    REG_POINTER,
    DEF_POINTER,

    // Constants
    DEF_CONST,
    NUM_CONST,

    // Other
    STRING
} Arguments;

// Registers
typedef enum
{
    R_NONE,

    A,
    B,
    C,
    D,
    E,
    H,
    L,
    REG16,
    HL,
    BC,
    DE,

    SP
} Registers;

// Argument union
union ARGct
{
    WORD        i;
    char*       s;
};

// Parsed assembly argument
class ASMArgument
{
    public:
        // Init function
        void                init(std::string);
        // Raw string
        std::string         raw;
        // Argument type
        Arguments           argtype;
        // Argument content
        union ARGct         content;
};

// Parsed assembly command
class ASMCommand
{
    public:
        // Init function
        void                init(std::string, size_t);
        // Check if the command matches a template
        bool                match(
                const Commands, const size_t, ...);
        // Raw string
        std::string         raw;
        // Line number
        size_t              linenum;
        // Command type
        Commands            cmd;
        // Number of arguments
        size_t              arsize;
        // Arguments
        ASMArgument         *args;
};

// Parsed assembly file
class ASMFile
{
    public:
        // Init function
        ASMFile             (std::string);
        // File name
        std::string         filename;
        // Number of lines
        size_t              lsize;
        // Array of parsed commands
        ASMCommand*         commands;
};

// Binary file
class ASMBinary
{
    public:
        // Init function
        ASMBinary           (std::string);
        // File name
        std::string         filename;
        // File content
        std::string         content;
};
#endif
