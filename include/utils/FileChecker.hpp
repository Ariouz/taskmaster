#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include "Logger.hpp"
#include "Config.hpp"
#include "Shell.hpp"
#include "Process.hpp"

class FileChecker {

    public:
        static void                                 checkFile( const std::string& configFile );
        static void                                 yamlComparator( ProcessManager& pm,
                                                        std::map<std::string, ProgramConfig> toCompare );
        static void                                 yamlComparatorElements( std::set<std::string>& commonKeys,
                                                        std::map<std::string, ProgramConfig> base,
                                                        std::map<std::string, ProgramConfig> toCompare );
        static void                                 yamlComparatorElementsDelete( ProcessManager& pm,
                                                        std::set<std::string> commonKeys,
                                                        std::set<std::string> baseKeys,
                                                        std::set<std::string> toCompareKeys );
        static void                                 yamlComparatorElementsAddAndStart( ProcessManager& pm,
                                                        std::map<std::string, ProgramConfig> toCompare,
                                                        std::set<std::string> commonKeys,
                                                        std::set<std::string> baseKeys,
                                                        std::set<std::string> toCompareKeys );

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
