#include "Taskmaster.hpp"
#include "FileChecker.hpp"
#include "Config.hpp"
#include "Shell.hpp"

Taskmaster::Taskmaster(const std::string& configFile) {
    FileChecker::checkFile(configFile);
    this->_config = Config(configFile);
    Logger::info("Loaded taskmaster with config file " + configFile);
    _process_manager.init();
    this->_shell = Shell();
}

Taskmaster::~Taskmaster() = default;
