#include <stdio.h>
#include <stdarg.h>
#include "debug.h"

/* build_info - Displays various BUILD constants to stdout. */
void
build_info(void)
{
    printf("%s version %s, built on %s at %s for %s\n\n",
            BUILD_NAME, BUILD_VERSION, BUILD_DATE, BUILD_TIME, BUILD_PLATFORM);
}

/* debug_log - Logs printf style strings to stdout if debug mode is enabled. 
 * Pass the __FILE__ constant (defined as FNAME) to arg fname to see where
 * debug statements are coming from. */
void 
debug_log(const char * fname, const char * format, ...)
{ 
    if (!DEBUG_MODE) {
        return;
    }
    va_list args;
    va_start(args, format);
    printf("[%s] ", fname);
    vprintf(format, args);
    va_end(args);
}
