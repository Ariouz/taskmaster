#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#ifdef BONUS
// Get syslogs: journalctl -f -t taskmaster
# include <syslog.h>
#endif

class Logger {

    private:
        Logger(const std::string& logfile);

        std::string _logfile;
        bool        _verbose = true;

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

};
