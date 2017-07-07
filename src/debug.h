#ifndef DEBUG_H
#define DEBUG_H

// Set DEBUG_MODE whether DEBUG is passed via the compilation
// command or not.
#ifdef DEBUG
#define DEBUG_MODE 1
#else
#define DEBUG_MODE 0
#endif

// Define BUILD_PLATFORM_[WIN32|UNIX] based on platform.
#if defined(_WIN32)
#define BUILD_PLATFORM "WIN_32"
#define BUILD_PLATFORM_WIN32
#endif

#if defined(__unix__) || defined(__unix) || \
    (defined(__APPLE__) && defined(__MACH__))
#define BUILD_PLATFORM "UNIX"
#define BUILD_PLATFORM_UNIX
#endif

// __FILE__ constant shorthand for debug_print function.
#define FNAME __FILE__ 

// UNUSED constant to quiet compiler warnings.
#define UNUSED __attribute__((unused))

#define BUILD_NAME "Abra"
#define BUILD_VERSION "1.0"
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

void build_info(void);
void debug_log(const char * fname, const char * format, ...);

#endif
