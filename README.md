## Logging System (C++ Standalone Class)

The project includes a standalone `Log` class that can be reused in any C++ project.  
It provides a flexible, level-based logging system with both terminal output and file logging support.

It is designed with:
- Singleton access
- Compile-time log filtering
- Runtime log file generation
- Colored terminal output
- Automatic log file

---

## Features

- Singleton logger (`Log::instance()`)
- Multiple log levels:
  - SYSTEM ERROR
  - ERROR
  - WARNING
  - INFO
  - DEBUG
  - LOG
- Compile-time filtering via `LOG_LEVEL` and `PRINT_LEVEL`
- Automatic log file creation in `log/` directory
- Timestamped log files
- Thread-safe design not required (single-thread assumption typical 42 projects)

---

## Usage

### Basic initialization

```C++
#include "Log.hpp"

Log& logger = Log::instance();

if (!logger.getStatus()) {
    std::cerr << "Log failed to setup" << std::endl;
    return FAILURE;
}
```

---

### Recommended usage via macros

The class is designed to be used mainly through macros:

```C++
LOG_ERROR("Failed to open file: " << filename);
LOG_WARNING("Low memory detected");
LOG_INFO("Server started on port " << port);
LOG_DEBUG("Request received: " << request);
LOG_LOG("Generic log message");
LOG_ERROR_SYS("System call failed");
LOG_HERE("Debug checkpoint reached");
```

---

## Log Levels

### Available levels

- `LVL_ERROR_SYSTEM`
- `LVL_ERROR`
- `LVL_WARNING`
- `LVL_INFO`
- `LVL_DEBUG`
- `LVL_LOG`

---

### Compile-time control

You can control output using:

```C++
#define LOG_LEVEL   (LVL_ERROR_SYSTEM | LVL_ERROR | LVL_DEBUG | LVL_LOG)
#define PRINT_LEVEL (LVL_ERROR_SYSTEM | LVL_ERROR | LVL_WARNING | LVL_INFO)
```

This allows:
- disabling logs in production
- keeping only critical errors
- enabling debug logs during development

---

## Log Output

### Terminal output

Logs are printed with colored prefixes depending on severity.

### File output

Logs are stored in:

```text
log/webserv_log_YYYYMMDD_HHMMSS.log
```

Each run generates a new timestamped file automatically.

---

## Behavior Details

- If the `log/` directory does not exist, it is created automatically
- If file creation fails, logging is disabled safely
- ANSI color codes are removed from file output
- System errors append `strerror(errno)` automatically
- Logging uses low-level `write()` for file output

---

## Architecture Notes

The logging system is composed of:

- Singleton `Log` instance
- Internal file descriptor (`_fd`)
- Log level filtering via macros
- Stream-based macro interface (`std::ostringstream`)
- Runtime-safe initialization (`createLogging()`)

---

## Example

```C++
LOG_INFO("Server started successfully");
LOG_DEBUG("Listening on port " << port);
LOG_ERROR_SYS("Socket creation failed");
```

---

## Summary

This logger provides a lightweight, reusable, and configurable logging system suitable for:
- network servers
- system-level projects
- debugging-heavy applications

It is fully decoupled from the Webserv project and can be reused independently.