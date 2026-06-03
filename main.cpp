#include "Log_with_time/Log.hpp"
#include <iostream>

int main(int ac, char** av)
{

	Log& logger = Log::instance();
	if (!logger.getStatus()) {
		std::cerr << RED "Log failed to setup" RESET << std::endl;
		return 0;
	}

    std::string filename = "path/to/file";
    std::string request = "HELLO";
    int port = 42;

    errno = EACCES;
    LOG_ERROR_SYS("System call failed");
    LOG_ERROR("Failed to open file: " C_321 << filename);
    LOG_WARNING("Low memory detected");
    LOG_INFO("Server started on port " C_321 << port);
    LOG_DEBUG("Request received: " C_321 << request);
    LOG_LOG("Generic log message");
    LOG_HERE("Debug checkpoint reached");

    std::cout << std::endl;
    return 0;
}