/*
 * VICasm Assembler by h34ting4ppliance
 *
 * logging.h
 *
 * Logging header file.
 */
#ifndef LOGGING_H
#define LOGGING_H

class Logging
{
    private:
        const char* filename;
    public:
        Logging::Logging (const char* fname);
        void log(const char* message, ...);
        void warn(const char* messasge, ...);
        void die(const char* message, ...);
}
