#include "Config.hpp"
#include "Logger.hpp"


////////// Constructors | Destructor ////////// 

Config::Config( void ) = default;

Config::Config(const std::string& filePath) : _filePath(filePath) {
    Logger::info("Loading config file...");
    load();
}

Config::~Config() = default;


////////// Getters ////////// 

std::map<std::string, ProgramConfig>    Config::getPrograms( void ) const {
    return this->_programs;
}


////////// Functions ////////// 

void    Config::load() {
    YAML::Node config = YAML::LoadFile(this->_filePath);

    YAML::Node programs = config["programs"];
    for (auto it = programs.begin(); it != programs.end(); ++it) {
        std::string program_name = it->first.as<std::string>();
        YAML::Node node = it->second;

        ProgramConfig progconf(program_name, node);
        this->_programs[program_name] = progconf;
    }

}
