#include "log.h"
#include <cstdarg>
#include <stdio.h>

int log_verbose = 0;


void logv(const int spec, const char *msg, ...) {
    if(log_verbose != spec) {
        return;
    }
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
    fflush(stdout);
}

void logv(const char *msg, ...) {
    if(log_verbose != 2) {
        return;
    }
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
    fflush(stdout);
}


void loge(const char *msg, ...) {
    if(!log_verbose) {
        return;
    }
    va_list args;
    va_start(args, msg);
    vfprintf(stderr,msg, args);
    va_end(args);
}
