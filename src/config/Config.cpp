#include "Config.hpp"

Config::Config(const std::string& filePath) : _filePath(filePath) {
    std::cout << "Loading config file ..." << std::endl;
    //load();
}

Config::~Config() = default;