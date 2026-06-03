#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <unordered_map>

#include "_colors.h"

// =========================================================
// LOG LEVELS (unchanged idea, but clean grouping)
// =========================================================

#define LVL_NONE          0
#define LVL_ERROR_SYSTEM  (1 << 0)
#define LVL_ERROR         (1 << 1)
#define LVL_WARNING       (1 << 2)
#define LVL_INFO          (1 << 3)
#define LVL_DEBUG         (1 << 4)
#define LVL_LOG           (1 << 5)

// =========================================================
// OUTPUT CONTROL
// =========================================================

#ifndef PRINT_LEVEL
# define PRINT_LEVEL (LVL_ERROR_SYSTEM | LVL_ERROR | LVL_WARNING | LVL_INFO | LVL_DEBUG | LVL_LOG)
#endif

#ifndef LOG_LEVEL
# define LOG_LEVEL (LVL_ERROR_SYSTEM | LVL_ERROR | LVL_DEBUG | LVL_LOG)
#endif

// =========================================================
// TIME MODES (NEW)
// =========================================================

#define TIME_NONE   0
#define TIME_ABS    (1 << 0)
#define TIME_UPTIME (1 << 1)
#define TIME_DELTA  (1 << 2)

#ifndef TIME_MODE
# define TIME_MODE (TIME_ABS | TIME_UPTIME | TIME_DELTA)
#endif

#define LOG_PATH "log/"

// =========================================================
// PREFIXES (terminal)
// =========================================================

#define ERROR_SYS_P "[ " C_510 "- SYS ERROR -" RESET " ] "
#define ERROR_P     "[ " C_510 " ERROR " RESET " ] "
#define WARNING_P   "[ " C_441 " WARNING " RESET " ] "
#define INFO_P      "[ " C_150 " INFO " RESET " ] "
#define DEBUG_P     "[ " C_124 " DEBUG " RESET " ] "
#define LOG_P       "[ " C_035 " LOG " RESET " ] "
#define OTHER_P     "<<<" C_520 " HERE " RESET ">>> "

// =========================================================
// CLASS
// =========================================================

class Log {
private:
    int  _fd;
    bool _status;

    using Clock = std::chrono::steady_clock;

    Clock::time_point _start;
    std::unordered_map<int, Clock::time_point> _last_by_level;

private:
    Log();
    ~Log();

public:
    static Log& instance();

    bool getStatus() const;

    // unified engine
    void log(int level, const std::ostringstream& msg);

    // convenience wrapper
    static void log_here(const std::ostringstream& msg);

private:
    bool openFile(const std::string& path);
    bool createLogging(const std::string& prefix = "webserv_log");
    std::string removeColors(const std::ostringstream& msg);

    std::string getTimePrefix(int level);
    const char* getLevelPrefix(int level);
};