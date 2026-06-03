#ifndef DEFINE_LEVELS_HPP
#define DEFINE_LEVELS_HPP

/* ===============================================================================
    LOGGER CONFIGURATION HEADER

This file defines:
- Which log levels are enabled
- Where they are printed (file / terminal)
- Which timestamps are used and where

You can safely modify values below to fit your needs.

=============================================================================== */

// ( LVL_ERROR_SYSTEM | LVL_ERROR | LVL_WARNING | LVL_INFO | LVL_DEBUG | LVL_LOG )
#define ALL ( LVL_ERROR_SYSTEM | LVL_ERROR | LVL_WARNING | LVL_INFO | LVL_DEBUG | LVL_LOG )
#define NONE 0

/* ===============================================================================

    OUTPUT CONTROL

=============================================================================== */

/* -------------------------------------------------- >
LOG_LEVEL:
    → Which levels are written to the log file
-------------------------------------------------- > */
#ifndef LOG_LEVEL
    #define LOG_LEVEL ( LVL_ERROR_SYSTEM | LVL_ERROR | LVL_DEBUG | LVL_LOG )
#endif

/* -------------------------------------------------- >
PRINT_LEVEL:
    → Which levels are printed to the terminal
-------------------------------------------------- > */

#ifndef PRINT_LEVEL
    #define PRINT_LEVEL ( LVL_ERROR_SYSTEM | LVL_ERROR | LVL_WARNING | LVL_INFO | LVL_DEBUG | LVL_LOG )
#endif

/* ===============================================================================

    TIME MODE CONFIGURATION

=============================================================================== */

/* -------------------------------------------------- >
PRINT_TIME_MODE:
    → What time information is shown in terminal output

    Available flags:
        T_ABS   → absolute time (HH:MM:SS)
        T_SINCE → time since program start
-------------------------------------------------- > */

#ifndef PRINT_TIME_MODE
    #define PRINT_TIME_MODE ( T_ABS )
#endif

/* -------------------------------------------------- >
LOG_TIME_MODE:
    → What time information is written to log files
-------------------------------------------------- > */

#ifndef LOG_TIME_MODE
    #define LOG_TIME_MODE ( T_ABS | T_SINCE )
#endif

/* -------------------------------------------------- >
PRINT_TIME_FILTER:
    → Which log levels include timestamps in terminal output
-------------------------------------------------- > */

#ifndef PRINT_TIME_FILTER
    #define PRINT_TIME_FILTER ( LVL_INFO | LVL_DEBUG | LVL_LOG )
#endif

/* -------------------------------------------------- >
LOG_TIME_FILTER:
    → Which log levels include timestamps in log file
-------------------------------------------------- > */

#ifndef LOG_TIME_FILTER
    #define LOG_TIME_FILTER ( LVL_ERROR | LVL_WARNING | LVL_INFO | LVL_DEBUG | LVL_LOG )
#endif

/* -------------------------------------------------- >
LOG_HERE_TIME_FILTER:
    → Special case for LOG_HERE macro/function
    → Defines which time modes are applied (independent of PRINT/LOG modes)

    Available flags:
        T_SINCE → time since program start
        T_DELTA → time since previous LOG_HERE
-------------------------------------------------- > */

#ifndef LOG_HERE_TIME_FILTER
    #define LOG_HERE_TIME_FILTER ( T_SINCE | T_DELTA )
#endif

#endif