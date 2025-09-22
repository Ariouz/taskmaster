#pragma once

#include <map>
#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>
#include "ProgramConfig.hpp"

class Config {
	private:
		std::map<std::string, ProgramConfig> _programs;
		std::string _filePath;

	public:
		Config( void );
		Config(const std::string& path);
		~Config();

        std::map<std::string, ProgramConfig>    getPrograms( void ) const;

		void load();
		void reload();
};
