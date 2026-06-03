#include "Log.hpp"
#include "_colors.h"

#include <iomanip>

// --------------------------------------------------------------------------- >
std::string Log::getCurrentTime(bool isFile)
{
    std::time_t now = std::time(NULL);
    std::tm tm;

    localtime_r(&now, &tm);

    std::ostringstream oss;

    if (isFile)
    {
        // NO COLORS
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    }
    else
    {
        // TERMINAL VERSION (you can add colors here)
        oss << C_510
            << std::put_time(&tm, "%H:%M:%S")
            << RESET;
    }
    return oss.str();
}

// --------------------------------------------------------------------------- >
std::string Log::buildTime(bool isFile, int lvl)
{
    // correct filter usage
    int filter = isFile ? LOG_TIME_FILTER : PRINT_TIME_FILTER;

    if (!(filter & lvl))
        return "";

    int mode = isFile ? LOG_TIME_MODE : PRINT_TIME_MODE;

    std::ostringstream oss;

    // 1. absolute time
    if (mode & T_ABS)
        oss << "[" << getCurrentTime(isFile) << "]";

    // 2. since start
    if (mode & T_SINCE) {
        if (isFile)
            oss << "[+" << getSinceStart() << "ms]";
        else
            oss << "[+" C_431 << getSinceStart() << RESET "ms]";
    }

    return oss.str();
}

// --------------------------------------------------------------------------- >
double Log::getSinceStart()
{
    auto now = std::chrono::steady_clock::now();

    return std::chrono::duration<double>(now - _start).count() * 1000.0;
}