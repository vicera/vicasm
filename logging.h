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
        Logging (const char* fname);
        void log(const char* msg, ...);
        void warn(const char* messasge, ...);
        void error(const char* msg, ...);
};

#endif
