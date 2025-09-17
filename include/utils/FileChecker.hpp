#include <filesystem>
#include <iostream>
#include <fstream>

class FileChecker {

    public:
        static void    checkFile( const std::string& configFile );

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