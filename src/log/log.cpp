#include <cstdarg>
#include <cstdio>

#include "log.h"


int log_verbose = 0;

void logv(const int spec, const char *msg, ...) {
#ifndef NDEBUG
    if(log_verbose != spec) {
        return;
    }
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
    fflush(stdout);
#endif
}

void logv(const char *msg, ...) {
#ifndef NDEBUG
    if(log_verbose != 2) {
        return;
    }
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
    fflush(stdout);
#endif
}

void loge(const char *msg, ...) {
#ifndef NDEBUG
    if(!log_verbose) {
        return;
    }
    va_list args;
    va_start(args, msg);
    vfprintf(stderr,msg, args);
    va_end(args);
#endif
}
