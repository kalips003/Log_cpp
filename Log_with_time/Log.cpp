#include "Log.hpp"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctime>
#include <iomanip>

Log& Log::instance() {
    static Log l;
    return l;
}

Log::Log()
    : _fd(-1), _status(true), _start(Clock::now())
{
    createLogFile();
}

Log::~Log() {
    if (_fd >= 0)
        close(_fd);
}

bool Log::getStatus() const {
    return _status;
}

const char* Log::prefix(int level) {
    if (level == LVL_ERROR_SYSTEM) return "[SYS]";
    if (level == LVL_ERROR)        return "[ERR]";
    if (level == LVL_WARNING)      return "[WRN]";
    if (level == LVL_INFO)         return "[INF]";
    if (level == LVL_DEBUG)        return "[DBG]";
    if (level == LVL_LOG)          return "[LOG]";
    return "[???]";
}

std::string Log::timestamp() {
    std::time_t t = std::time(NULL);
    std::tm tm;
    localtime_r(&t, &tm);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S");
    return oss.str();
}

void Log::log(int level, const std::string& msg) {

    auto now = Clock::now();

    double delta = 0.0;
    if (_last.count(level))
        delta = std::chrono::duration<double>(now - _last[level]).count();
    _last[level] = now;

    std::ostringstream out;
    out << prefix(level)
        << "[" << timestamp() << "]"
        << "[+" << delta << "s] "
        << msg;

    std::cout << out.str() << std::endl;

    if (_fd >= 0) {
        std::string file = out.str() + "\n";
        write(_fd, file.c_str(), file.size());
    }
}

void Log::log_here(const std::string& msg) {
    std::cerr << "<<< HERE >>> " << msg << std::endl;
}

bool Log::createLogFile() {
    _fd = open("log.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
    _status = (_fd >= 0);
    return _status;
}