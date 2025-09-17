#include "FileChecker.hpp"

void    FileChecker::checkFile( const std::string& configFile ) {
    std::filesystem::path filePath(configFile);

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

const char *FileChecker::NoFileException::what( void ) const throw() {
	return "Please enter the name of the configuration file (.yml or .yaml) as an argument.";
}

const char *FileChecker::NotRegularFileException::what( void ) const throw() {
	return "The path does not point to a regular file.";
}

const char *FileChecker::ReadableFileException::what( void ) const throw() {
	return "The file exists but cannot be read (see file permissions).";
}
