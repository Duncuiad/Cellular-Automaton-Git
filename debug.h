#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#ifndef DEBUG
#define DEBUG 1
#endif

#include <stdarg.h>
#include <stdio.h>

#define TRACE(x) do { if (DEBUG) dbg_printf x; } while (0)
/* USAGE
** TRACE(("Messaggio: %d %f\n", i, f));
*/

void dbg_printf(const char *fmt, ... );

#endif /*DEBUG_H_INCLUDED*/
