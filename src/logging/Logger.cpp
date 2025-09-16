#include "Logger.hpp"

Logger::Logger() = default;
Logger::Logger(const std::string& logfile) : _logfile(logfile) {}
Logger::~Logger() = default;