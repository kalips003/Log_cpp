#include "Log.hpp"

#include <iostream>
#include <unistd.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////]
/***						LOG FUNCTIONS					 				***/
///////////////////////////////////////////////////////////////////////////////]

void Log::log_here(const std::ostringstream& msg)
{
    std::cerr << OTHER_P;

#if LOG_HERE_TIME_FILTER & T_SINCE
    std::cerr << "[+" C_250 << getSinceStart() << RESET "s]";
#endif

#if LOG_HERE_TIME_FILTER & T_DELTA
    static auto last = std::chrono::steady_clock::now();
    static bool first = true;

    auto now = std::chrono::steady_clock::now();

    double delta = 0.0;

    if (first)
        first = false;
    else
        delta = std::chrono::duration<double>(now - last).count() * 1000.0;

    last = now;
    std::cerr << "[Δ" C_431 << delta << RESET "ms] ";
#endif

    std::cerr << " " << msg.str() << std::endl;
}


//-----------------------------------------------------------------------------]
void Log::log_error_sys(const std::ostringstream& msg) {
#if LOG_LEVEL & LVL_ERROR_SYSTEM
    std::string time_file = buildTime(true, LVL_LOG);

	std::string s = ERROR_SYS_F + time_file + " " + removeColors(msg) + " (" + strerror(errno) + ")\n";
	if (_fd >= 0)
		write(_fd, s.c_str(), s.size());
#endif

#if PRINT_LEVEL & LVL_ERROR_SYSTEM
    std::string time_term = buildTime(false, LVL_LOG);

	std::cerr << ERROR_SYS_P << time_term << " " << msg.str() << " (" << strerror(errno) << ")\n";
#endif
	(void)msg;
}

//-----------------------------------------------------------------------------]
void Log::log_error(const std::ostringstream& msg) {
#if LOG_LEVEL & LVL_ERROR
    std::string time_file = buildTime(true, LVL_LOG);

	std::string s = ERROR_F + time_file + " " + removeColors(msg) + "\n";
	if (_fd >= 0)
		write(_fd, s.c_str(), s.size());
#endif

#if PRINT_LEVEL & LVL_ERROR
    std::string time_term = buildTime(false, LVL_LOG);

	std::cout << ERROR_P << time_term << " " << msg.str() << std::endl;
#endif
	(void)msg;
}


//-----------------------------------------------------------------------------]
void Log::log_warning(const std::ostringstream& msg) {
#if LOG_LEVEL & LVL_WARNING
    std::string time_file = buildTime(true, LVL_LOG);

	std::string s = WARNING_F + time_file + " " + removeColors(msg) + "\n";
	if (_fd >= 0)
		write(_fd, s.c_str(), s.size());
#endif

#if PRINT_LEVEL & LVL_WARNING
    std::string time_term = buildTime(false, LVL_LOG);

	std::cout << WARNING_P << time_term << " " << msg.str() << std::endl;
#endif
	(void)msg;
}

//-----------------------------------------------------------------------------]
void Log::log_info(const std::ostringstream& msg) {
#if LOG_LEVEL & LVL_INFO
    std::string time_file = buildTime(true, LVL_LOG);

	std::string s = INFO_F + time_file + " " + removeColors(msg) + "\n";
	if (_fd >= 0)
		write(_fd, s.c_str(), s.size());
#endif

#if PRINT_LEVEL & LVL_INFO
    std::string time_term = buildTime(false, LVL_LOG);

	std::cout << INFO_P << time_term << " " << msg.str() << std::endl;
#endif
	(void)msg;
}

//-----------------------------------------------------------------------------]
void Log::log_debug(const std::ostringstream& msg) {
#if LOG_LEVEL & LVL_DEBUG
    std::string time_file = buildTime(true, LVL_LOG);

	std::string s = DEBUG_F + time_file + " " + removeColors(msg) + "\n";
	if (_fd >= 0)
		write(_fd, s.c_str(), s.size());
#endif

#if PRINT_LEVEL & LVL_DEBUG
    std::string time_term = buildTime(false, LVL_LOG);

	std::cout << DEBUG_P << time_term << " " << msg.str() << std::endl;
#endif
	(void)msg;
}

//-----------------------------------------------------------------------------]
void Log::log_log(const std::ostringstream& msg)
{
#if LOG_LEVEL & LVL_LOG
    std::string time_file = buildTime(true, LVL_LOG);

    std::string s = LOG_F + time_file + " " + removeColors(msg) + "\n";

    if (_fd >= 0)
        write(_fd, s.c_str(), s.size());
#endif

#if PRINT_LEVEL & LVL_LOG
    std::string time_term = buildTime(false, LVL_LOG);

    std::cout << LOG_P << time_term << " " << msg.str() << std::endl;
#endif
	(void)msg;
}
