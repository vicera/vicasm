/*
 * VICERA by h34ting4ppliance
 * VICasm by h34ting4ppliance
 *
 * logging.cc
 *
 * This file contains all the logging functions to make log formatting easier.
 * This file has been ported to C++ for the assembler.
 */
#include <cstdio>
#include <cstdlib>
#include <cstdarg>

#include "logging.h"

// Init function
Logging::Logging(const char* fname)
{
    filename = fname;
}

// String, String -> None
// writes log
void Logging::log(const char* msg, ...)
{
    fprintf(stderr, "\033[1m[#] |%s|\033[0m ", filename);

    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);

    fprintf(stderr, "\n");
}

// String, String -> None
// warning log
void Logging::warn(const char* msg, ...)
{
    fprintf(stderr, "\033[1;43m[!] |%s|\033[0;43m ", filename);
    
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);

    fprintf(stderr, "\033[0m\n");
}

// String, String -> None
// writes error
void Logging::error(const char* msg, ...)
{
    fprintf(stderr, "\033[1;41m[X] |%s|\033[0;41m ", filename);
    
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);

    fprintf(stderr, "\033[0m\n");
}
