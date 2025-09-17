#include "Taskmaster.hpp"
#include "FileChecker.hpp"
#include "Config.hpp"

Taskmaster::Taskmaster(const std::string& configFile) {
    FileChecker::checkFile(configFile);
    this->_config = Config(configFile);
    Logger::info("Loaded taskmaster with config file " + configFile);
    _process_manager.init();
}

Taskmaster::~Taskmaster() = default;
