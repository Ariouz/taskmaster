#include "Taskmaster.hpp"

Taskmaster::Taskmaster(const std::string& configFile) : _config(configFile) {
    Logger::info("Loaded taskmaster with config file " + configFile);
}
Taskmaster::~Taskmaster() = default;