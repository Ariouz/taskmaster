#pragma once

#include <map>
#include <iostream>
#include <string>
#include "ProgramConfig.hpp"

class Config {
	private:
		std::map<std::string, ProgramConfig> _programs;
		std::string _filePath;

	public:
		Config(const std::string& path);
		~Config();

		void load();
		void reload();

		const ProgramConfig& getProgram(const std::string& name) const;
		const std::map<std::string, ProgramConfig>& getAllPrograms() const;

		bool hasProgram(const std::string& name) const;
};
