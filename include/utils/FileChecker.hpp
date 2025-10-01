#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include "Logger.hpp"
#include "Config.hpp"
#include "Shell.hpp"

class FileChecker {

    public:
        static void                                 checkFile( const std::string& configFile );
        static std::map<std::string, ProgramConfig> yamlComparator( ProcessManager& pm,
                                                        std::map<std::string, ProgramConfig> toCompare );
        static void                                 yamlComparatorElements( std::set<std::string>& commonKeys,
                                                        std::map<std::string, ProgramConfig> base,
                                                        std::map<std::string, ProgramConfig> toCompare );

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
