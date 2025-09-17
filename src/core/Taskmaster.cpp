#include "Taskmaster.hpp"

Taskmaster::Taskmaster(const std::string& configFile) : _config(configFile) {
    fileChecker();
    Logger::info("Loaded taskmaster with config file " + configFile);
}

Taskmaster::~Taskmaster() = default;

void    Taskmaster::fileChecker( void ) const {
    std::filesystem::path filePath(this->_configFile);

    if (!std::filesystem::is_regular_file(filePath)) {
        throw NotRegularFileException();
    }

    if (!(filePath.extension() == ".yaml" || filePath.extension() == ".yml"))
        throw NoFileException();

    if (!std::filesystem::exists(filePath))
        throw NoFileException();

    std::ifstream file(filePath);
    if (!file.good())
        throw ReadableFileException();
    
}

const char *Taskmaster::NoFileException::what( void ) const throw() {
	return "Please enter the name of the configuration file (.yml or .yaml) as an argument.";
}

const char *Taskmaster::NotRegularFileException::what( void ) const throw() {
	return "The path does not point to a regular file.";
}

const char *Taskmaster::ReadableFileException::what( void ) const throw() {
	return "The file exists but cannot be read (see file permissions).";
}
