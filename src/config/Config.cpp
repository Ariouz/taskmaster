#include "Config.hpp"
#include "Logger.hpp"

Config::Config(const std::string& filePath) : _filePath(filePath) {
    Logger::info("Loading config file...");
    //load();
}

Config::~Config() = default;