#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>
#include <unordered_map>

#include "_colors.h"

#define LVL_ERROR_SYSTEM  (1 << 0)
#define LVL_ERROR         (1 << 1)
#define LVL_WARNING       (1 << 2)
#define LVL_INFO          (1 << 3)
#define LVL_DEBUG         (1 << 4)
#define LVL_LOG           (1 << 5)

#ifndef PRINT_LEVEL
#define PRINT_LEVEL (LVL_ERROR_SYSTEM | LVL_ERROR | LVL_WARNING | LVL_INFO | LVL_DEBUG | LVL_LOG)
#endif

#ifndef LOG_LEVEL
#define LOG_LEVEL (LVL_ERROR_SYSTEM | LVL_ERROR | LVL_DEBUG | LVL_LOG)
#endif

#define LOG_PATH "log/"

class Log {
public:
    static Log& instance();

    bool getStatus() const;

    void log(int level, const std::string& msg);
    void log_here(const std::string& msg);

private:
    Log();
    ~Log();

    bool createLogFile();
    std::string timestamp();
    const char* prefix(int level);

private:
    int _fd;
    bool _status;

    using Clock = std::chrono::steady_clock;
    Clock::time_point _start;
    std::unordered_map<int, Clock::time_point> _last;
};

// ===================== MACROS =====================

#define LOG_STREAM(level, x) do { \
    std::ostringstream oss; oss << x; \
    Log::instance().log(level, oss.str()); \
} while(0)

#define LOG_ERROR(x)   LOG_STREAM(LVL_ERROR, x)
#define LOG_ERROR_SYS(x) LOG_STREAM(LVL_ERROR_SYSTEM, x)
#define LOG_WARNING(x) LOG_STREAM(LVL_WARNING, x)
#define LOG_INFO(x)    LOG_STREAM(LVL_INFO, x)
#define LOG_DEBUG(x)   LOG_STREAM(LVL_DEBUG, x)
#define LOG_LOG(x)     LOG_STREAM(LVL_LOG, x)

#define LOG_HERE(x) do { \
    std::ostringstream oss; oss << x; \
    Log::instance().log_here(oss.str()); \
} while(0)