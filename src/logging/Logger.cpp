#include "Logger.hpp"

Logger::Logger() = default;
Logger::Logger(const std::string& logfile) : _logfile(logfile) {
    std::ofstream ofs(logfile, std::ofstream::out | std::ofstream::trunc);
    if (!ofs.is_open()) {
        throw std::runtime_error("Failed to open logfile : " + logfile);
    }
}
Logger::~Logger() = default;

void Logger::setVerbose(bool verbose) {
    instance()._verbose = verbose;
}

void Logger::info(const std::string& msg) { 
    instance()._log(COLOR_INFO"[INFO] ", COLOR_RESET+msg); 
    #ifdef BONUS
    openlog("taskmaster", LOG_PID | LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "%s", msg.c_str());
    closelog();
    #endif
}

void Logger::warn(const std::string& msg) { 
    instance()._log(COLOR_WARN"[WARN] ", COLOR_RESET+msg); 
    #ifdef BONUS
    openlog("taskmaster", LOG_PID | LOG_CONS, LOG_USER);
    syslog(LOG_WARNING, "%s", msg.c_str());
    closelog();
    #endif
}

void Logger::error(const std::string& msg) { 
    instance()._log(COLOR_ERROR"[ERROR] ", COLOR_RESET+msg);
    #ifdef BONUS
    openlog("taskmaster", LOG_PID | LOG_CONS, LOG_USER);
    syslog(LOG_ERR, "%s", msg.c_str());
    closelog();
    #endif
}


void Logger::_log(const std::string& level, const std::string& msg) {
    std::ofstream ofs(_logfile, std::ofstream::out | std::ofstream::app);
    std::stringstream ss;
    ss << level << msg << std::endl;
    ofs << ss.str();
    if (_verbose) {
        if (level == "[ERROR] ") std::cerr << ss.str();
        else std::cout << ss.str();
    }
}