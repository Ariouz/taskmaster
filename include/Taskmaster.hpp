#pragma once

#include "Config.hpp"
#include "ProcessManager.hpp"
#include "Shell.hpp"
#include "Logger.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>

class Taskmaster {

    private:
        Config          _config;
        ProcessManager  _process_manager;
        Shell           _shell;
        Logger          _logger;
        std::string     _configFile;

    public:
        Taskmaster(const std::string& configFile);
        ~Taskmaster();

        void    fileChecker( void ) const;

        class NoFileException : public std::exception {
			public:
				virtual const char *what( void ) const throw();
		};

        class NotRegularFileException : public std::exception {
			public:
				virtual const char *what( void ) const throw();
		};

        class ReadableFileException : public std::exception {
			public:
				virtual const char *what( void ) const throw();
		};

};
