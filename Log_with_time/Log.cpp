#include "Log.hpp"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

// =========================================================
// SINGLETON
// =========================================================

Log& Log::instance() {
    static Log logger;
    return logger;
}

// =========================================================
// CONSTRUCTOR
// =========================================================

Log::Log()
    : _fd(-1), _status(true), _start(Clock::now())
{
#if LOG_LEVEL > LVL_NONE
    _status = createLogging();
#endif
}

Log::~Log() {
    if (_fd >= 0)
        close(_fd);
}

bool Log::getStatus() const {
    return _status;
}

// =========================================================
// CORE LOG FUNCTION (UNIFIED ENGINE)
// =========================================================

void Log::log(int level, const std::ostringstream& msg)
{
    if (!(_status))
        return;

    if (!(PRINT_LEVEL & level) && !(LOG_LEVEL & level))
        return;

    std::string out;

    // ---------- PREFIX ----------
    out += getLevelPrefix(level);
    out += getTimePrefix(level);
    out += " ";
    out += msg.str();

    // ---------- FILE ----------
    if (LOG_LEVEL & level && _fd >= 0)
    {
        std::string fileLine = removeColors(msg);
        fileLine = getLevelPrefix(level) + getTimePrefix(level) + " " + fileLine + "\n";
        write(_fd, fileLine.c_str(), fileLine.size());
    }

    // ---------- CONSOLE ----------
    if (PRINT_LEVEL & level)
        std::cout << out << std::endl;
}

// =========================================================
// TIME SYSTEM (THE GOOD PART)
// =========================================================

std::string Log::getTimePrefix(int level)
{
    std::ostringstream oss;

#if TIME_MODE & TIME_ABS
    std::time_t t = std::time(NULL);
    std::tm tm;
    localtime_r(&t, &tm);

    oss << "["
        << std::put_time(&tm, "%H:%M:%S")
        << "]";
#endif

#if TIME_MODE & TIME_UPTIME
    auto now = Clock::now();
    double sec = std::chrono::duration<double>(now - _start).count();

    oss << "[+" << std::fixed << std::setprecision(3)
        << sec << "s]";
#endif

#if TIME_MODE & TIME_DELTA
    auto now2 = Clock::now();

    double delta = 0.0;
    auto it = _last_by_level.find(level);

    if (it != _last_by_level.end())
        delta = std::chrono::duration<double>(now2 - it->second).count();

    _last_by_level[level] = now2;

    oss << "[Δ" << std::fixed << std::setprecision(3)
        << delta << "s]";
#endif

    return oss.str();
}

// =========================================================
// LEVEL PREFIX
// =========================================================

const char* Log::getLevelPrefix(int level)
{
    if (level & LVL_ERROR_SYSTEM) return ERROR_SYS_P;
    if (level & LVL_ERROR)        return ERROR_P;
    if (level & LVL_WARNING)      return WARNING_P;
    if (level & LVL_INFO)         return INFO_P;
    if (level & LVL_DEBUG)        return DEBUG_P;
    if (level & LVL_LOG)          return LOG_P;
    return OTHER_P;
}

// =========================================================
// REMOVE ANSI COLORS (FILE SAFE)
// =========================================================

std::string Log::removeColors(const std::ostringstream& msg)
{
    std::string out = msg.str();

    for (size_t i = 0; i < out.size(); )
    {
        if (out[i] == '\e')
        {
            size_t end = out.find('m', i);
            if (end == std::string::npos)
                break;
            out.erase(i, end - i + 1);
        }
        else
            ++i;
    }
    return out;
}

// =========================================================
// FILE SETUP (unchanged idea, cleaned)
// =========================================================

bool Log::createLogging(const std::string& prefix)
{
    struct stat st;

    if (stat(LOG_PATH, &st) != 0)
    {
        if (mkdir(LOG_PATH, 0777) != 0)
            return (_status = false);
    }
    else if (!S_ISDIR(st.st_mode))
        return (_status = false);

    if (access(LOG_PATH, W_OK) != 0)
        return (_status = false);

    std::time_t t = std::time(NULL);
    std::tm tm;
    localtime_r(&t, &tm);

    std::ostringstream path;
    path << LOG_PATH << prefix << "_"
         << (tm.tm_year + 1900)
         << std::setw(2) << std::setfill('0') << (tm.tm_mon + 1)
         << std::setw(2) << std::setfill('0') << tm.tm_mday
         << "_"
         << std::setw(2) << std::setfill('0') << tm.tm_hour
         << std::setw(2) << std::setfill('0') << tm.tm_min
         << std::setw(2) << std::setfill('0') << tm.tm_sec
         << ".log";

    return openFile(path.str());
}

bool Log::openFile(const std::string& path)
{
    _fd = open(path.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0666);
    _status = (_fd >= 0);
    return _status;
}

// =========================================================
// SMALL UTILITY
// =========================================================

void Log::log_here(const std::ostringstream& msg)
{
    std::cerr << OTHER_P << msg.str() << std::endl;
}