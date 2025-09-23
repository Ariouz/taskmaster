#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#ifdef BONUS
// Get syslogs: journalctl -f -t taskmaster
# include <syslog.h>

#define COLOR_SUCCESS "\033[32m"
#define COLOR_INFO "\033[36m"
#define COLOR_WARN "\033[33m"
#define COLOR_ERROR "\033[31m"
#define COLOR_UNKNOWN "\033[35m"
#define COLOR_RESET "\033[0m"
#else
#define COLOR_SUCCESS ""
#define COLOR_INFO ""
#define COLOR_WARN ""
#define COLOR_ERROR ""
#define COLOR_UNKNOWN ""
#define COLOR_RESET ""
#endif


class Logger {

    private:
        Logger(const std::string& logfile);

        std::string _logfile;
        bool        _verbose = false;

        void _log(const std::string& level, const std::string& msg);

    public:
        Logger();
        ~Logger();

        static Logger& instance() {
            static Logger _instance("taskmaster.log");
            return _instance;
        }

        static void info(const std::string& message);
        static void warn(const std::string& message);
        static void error(const std::string& message);

        static void setVerbose(bool verbose);

};
