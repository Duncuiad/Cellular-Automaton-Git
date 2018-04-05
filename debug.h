#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

/* THIS IS THE GLOBAL DEBUG TOGGLE. CAN BE OVERRIDDEN ON A PER-FILE BASIS */
#ifndef DEBUG
#define DEBUG 0 /*1 is debug active, 0 is not*/
#endif

#include <stdarg.h>
#include <stdio.h>

#define TRACE(x) do { if (DEBUG) dbg_printf x; } while (0)
/* USAGE
** TRACE(("Messaggio: %d %f\n", i, f));
*/

void dbg_printf(const char *fmt, ... );

#endif /*DEBUG_H_INCLUDED*/
