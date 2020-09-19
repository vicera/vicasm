#include <cstdio>
#include <cstdlib>
#include <cstddef>

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

#include "parser.h"

#define IS_BLANK(x) (x == ' ' || x == '\t' || x == '\n')

/*
 * Logging
 */

static void error(const char* message)
{
    printf("[xx] %s\n", message);
}

static void die(const char* message)
{
    error(message);
    perror(message);

    exit(1);
}

/*
 * Auxiliary functions
 */

// std::string -> std::string
// Puts file content into a string.
static inline std::string read_into_file(const std::string filename)
{
    std::ifstream ifs(filename);
    if (!ifs)
        die("File error.");

    // C++ is weird. Why does C has to have such shitty
    // string functions.
    std::string content(
            (std::istreambuf_iterator<char>(ifs)),
            (std::istreambuf_iterator<char>()));
    return content;
}

// std::string -> std::string
// Strips whitespace
static std::string strip(std::string a_str)
{
    size_t first = a_str.find_first_not_of(" \t\n");
    if (first == std::string::npos) first = 0;
    
    size_t last  = a_str.find_last_not_of(" \t\n");
    return a_str.substr(first, last - first + 1);
}

// std::string -> bool
// Starts with?
static inline bool starts_with(std::string str, std::string key)
{
    if (key.size() > str.size())
        return false;
    return str.find(key) == 0;
}

// std::string -> size_t
// Counts characters in a string.
static inline size_t count_chars(std::string str, const char c)
{
    return std::count(str.begin(), str.end(), c);
}

// std::string -> std::string
// Trim spaces
static inline std::string to_words(std::string a_str)
{
    a_str = strip(a_str);
    std::string str = "";

    for (size_t i = 0; i < a_str.size(); ++i)
        if ((IS_BLANK(a_str[i]) && !IS_BLANK(a_str[i+1]))
            || !IS_BLANK(a_str[i]))
            str += a_str[i];

    return str;
}

// std::string -> std::string
// Strips blanks and extracts comments
static inline std::string asm_strip(std::string a_str)
{
    std::string str = a_str.substr(0, a_str.find_first_of(';'));
    return to_words(str);
}

// std::string -> std::string*
// Splits strings
static std::string *string_split(std::string a_str, const char delim,
                                     size_t *a_sz)
{
    size_t sz = count_chars(a_str, delim) + 1;
    size_t curpos = 0;
    std::string str = a_str;
    std::string *result = new std::string[sz];

    for (size_t i = 0; i < sz; ++i)
    {
        curpos = str.find_first_of(delim);
        result[i] = str.substr(0, curpos);
        
        str = str.substr(curpos+1, -1);
    }
    
    *a_sz = sz;
    return result;
}

// std::string -> std::string
// Puts string in lowercase
static inline void str_tolower(std::string *str)
{
    std::transform(str->begin(), str->end(), str->begin(), ::tolower);
}

/*
 * Parsing intermediate functions
 */

// ASMCommand

// std::string -> Commands
// returns a command ID from string
Commands get_command(std::string a_str)
{
    std::string str = a_str;
    str_tolower(&str);

    if (str == "halt")          return HALT;
    else if (str == "nop")      return NOP;

    else if (str == "push")     return PUSH;
    else if (str == "pusha")    return PUSHA;
    else if (str == "pop")      return POP;
    else if (str == "popa")     return POPA;

    else if (str == "mov")      return MOV;
    else if (str == "add")      return ADD;
    else if (str == "sub")      return SUB;
    else if (str == "and")      return AND;
    else if (str == "or")       return OR;
    else if (str == "xor")      return XOR;
    
    else if (str == "inc")      return INC;
    else if (str == "dec")      return DEC;
    else if (str == "sl")       return SL;
    else if (str == "sr")       return SR;

    else if (str == "cp")       return CP;
    else if (str == "jp")       return JP;
    else if (str == "jc")       return JC;
    else if (str == "jz")       return JZ;
    else if (str == "jn")       return JN;
    else if (str == "call")     return CALL;
    else if (str == "ret")      return RET;

    else if (str == "dumpr")    return DUMP_R;
    else if (str == "dumpm")    return DUMP_M;

    else if (str == "slp")      return SLP;
    else if (str == "swap")     return SWAP;

    // Assembler-wide commands
    else if (str == ".define")  return DEFINE;
    else if (str == ".include") return INCLUDE;
    else if (str == ".bin")     return BIN;
    else if (str == ".org")     return ORG;
    else if (str == ".db")      return DB;
    else if (str.back() == ':') return LBL;

    // None.
    else
        return C_NONE;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/*
 *  std::string -> ASMArgument
 *
 *  ASMArgument Init function
 *
 *  Will parse a non-parsed argument string
 *  into an argument with it's type and it's content.
 *
 *  Examples:
 *      "0x1234" -> ASMArgument
 *                      .raw =      "0x1234"
 *                      .argtype =  NUM_CONST
 *                      .content =  4660
 *      "(HL)"   -> ASMArgument
 *                      .raw =      "(HL)"
 *                      .argtype =  REG_POINTER
 *                      .content =  HL
 */
ASMArgument::ASMArgument (std::string a)
{
    // Strips that shit
    std::string str = strip(a);
    

}

/*
 * std::string -> ASMCommand
 * ASMCommand Init function
 *
 * Will parse a line of assembly code into
 * it's command and it's arguments
 * 
 * Examples:
 *      "mov a, b" -> ASMCommand
 *                      .raw =      "mov a, b"
 *                      .cmd =      MOV
 *                      .args =     { ASMArgument, ASMArgument }
 */
void ASMCommand::init (std::string c, size_t l)
{
    raw     = c;
    linenum = l;
    
    // Seperate command from arguments
    std::string rcmd    = c.substr(0, c.find_first_of(' '));
    std::string rargs   = c.substr(c.find_first_of(' ')+1, -1);

    // Parse command
    cmd = get_command(rcmd);

    // TODO: Finish implementing this function.
}

/*
 * std::string -> ASMFile
 *
 * ASMFile Init function
 *
 * Will parse a whole ASM file into an array of commands.
 */
ASMFile::ASMFile (std::string fn)
{
    filename    = fn;
    std::string content = read_into_file(fn);

    size_t sz;
    std::string *lines = string_split(content, '\n', &sz);
    // Progressive initialization.
    commands           = new ASMCommand[sz];

    for (size_t i = 0; i < sz; ++i)
    {
        if ((lines[i]=asm_strip(lines[i])) != "")
            commands[i].init(lines[i], i+1);
            // TODO: Fix this shit.
    }
}

/*
 * std::string -> ASMBinary
 * ASMBinary Init function
 *
 * Will copy a Binary file into a string.
 */
ASMBinary::ASMBinary (const std::string fn)
{
    filename    = fn;
    content     = read_into_file(fn);
}
