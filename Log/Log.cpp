#include "Log.hpp"
#include "_colors.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////]
/** oss msg; msg << ""; Log::log(OTHER_P, msg.str()); */
void	Log::log(const std::string& prefix, const std::string& msg) {
	std::cout << prefix << msg << std::endl;
}

///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
Log::Log() : _fd(-1), _status(true), _start(std::chrono::steady_clock::now()) {
#if LOG_LEVEL > LVL_NONE
	_status = createLogging();
#endif
}

///////////////////////////////////////////////////////////////////////////////]
Log::~Log() {
	if (_fd >= 0) { close(_fd); _fd = -1; }
}

///////////////////////////////////////////////////////////////////////////////]
bool Log::createLogging(const std::string& prefix) {

	struct stat st;
// Exist? if not exist → try to create it
	if (stat(LOG_PATH, &st) != 0) {
		if (mkdir(LOG_PATH, 0777) != 0) {
			perror("mkdir(): " LOG_PATH);
			return (_status = false);
		}
	}
// is a directory
	else if (!S_ISDIR(st.st_mode))
		return (_status = false);
// can be accessed (and written to)
	if (access(LOG_PATH, W_OK) != 0) {// even if file exist, might not be readable by server
		perror("access(): " LOG_PATH);
		return (_status = false);
	}

// create time stamp
std::time_t t = std::time(NULL);
	std::tm tm;
	localtime_r(&t, &tm);
// webserv_log_yyyymmjjhhmmss.log
	std::ostringstream date;
	date << LOG_PATH << prefix << "_"
		<< (tm.tm_year + 1900)
		<< std::setw(2) << std::setfill('0') << (tm.tm_mon + 1)
		<< std::setw(2) << std::setfill('0') << tm.tm_mday
		<< "_"
		<< std::setw(2) << std::setfill('0') << tm.tm_hour
		<< std::setw(2) << std::setfill('0') << tm.tm_min
		<< std::setw(2) << std::setfill('0') << tm.tm_sec
		<< ".log";

	openFile(date.str());
	return _status;
}

///////////////////////////////////////////////////////////////////////////////]
void Log::openFile(const std::string& path) {
	_status = true;
	_fd = open(path.c_str(), O_CREAT | O_APPEND | O_WRONLY, 0666);
	if (_fd >= 0) 
		_status = true;
	else
		_status = false;
}

///////////////////////////////////////////////////////////////////////////////]
std::string		Log::removeColors(const std::ostringstream& msg) {
	std::string out = msg.str();

	for (size_t i = 0; i < out.size(); ) {
		if (out[i] == '\e') {
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
