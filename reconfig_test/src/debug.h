#ifndef _DEBUG_H
#define _DEBUG_H
 
//Turn on DEBUG printing
//#define DEBUG

#ifdef DEBUG
void dbprintf(const char *, ...);
#else
static inline void dbprintf(const char *fmt, ...){};
#endif 
 
#endif