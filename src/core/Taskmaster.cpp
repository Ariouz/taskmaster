#include "Taskmaster.hpp"
#include "FileChecker.hpp"

Taskmaster::Taskmaster(const std::string& configFile) : _config(configFile) {
    FileChecker::checkFile(configFile);
    Logger::info("Loaded taskmaster with config file " + configFile);
    _process_manager.init();
}

Taskmaster::~Taskmaster() = default;