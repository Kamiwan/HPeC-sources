#include <stdarg.h> /* va_end(), va_list, va_start(), vprintf() */
#include <stdio.h> /* vprintf() */
#include "debug.h"
 
#ifdef DEBUG
void dbprintf(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);
} 
#endif

