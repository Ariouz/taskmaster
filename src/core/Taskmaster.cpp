#include "Taskmaster.hpp"
#include "FileChecker.hpp"

Taskmaster::Taskmaster(const std::string& configFile) : _config(configFile) {
    FileChecker::checkFile(configFile);
    Logger::info("Loaded taskmaster with config file " + configFile);
}

Taskmaster::~Taskmaster() = default;