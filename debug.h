#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#ifndef DEBUG
#define DEBUG 0
#endif

#include <stdarg.h>
#include <stdio.h>

#define TRACE(x) do { if (DEBUG) dbg_printf x; } while (0)
/* USAGE
** TRACE(("Messaggio: %d %f\n", i, f));
*/

void dbg_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

#endif /*DEBUG_H_INCLUDED*/
