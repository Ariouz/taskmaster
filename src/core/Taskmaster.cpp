#include "Taskmaster.hpp"

Taskmaster::Taskmaster(const std::string& configFile) : _config(configFile), _logger("taskmaster.log") {
    std::cout << "Loaded taskmaster with config file " << configFile << std::endl; // TODO => USE LOGGER
}
Taskmaster::~Taskmaster() = default;