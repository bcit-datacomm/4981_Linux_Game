#ifndef LOG_H
#define LOG_H

extern int log_verbose;

//isaac morneau march 14, 2017
//only on when -v is enabled
void logv(const char *msg, ...);

//isaac morneau march 16, 2017
//only on when -o [spec] is enabled
void logv(const int spec, const char *msg, ...);

//isaac morneau march 14, 2017
//on whenever any verbose mode is on or when -e is used
void loge(const char *msg, ...);

#endif
