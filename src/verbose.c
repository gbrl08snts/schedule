#include <stdio.h>
#include <stdarg.h>

extern int VERBOSE;

// Function that wraps the printf function to check if VERBOSE is enabled
// before printing
void vprint(const char *fmt, ...) {
    if (!VERBOSE) 
        return;

    // handling the list of args
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args); // vprintf handles the va_list
    va_end(args);
}

