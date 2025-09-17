#include <stdexcept>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "Taskmaster.hpp"
#include "FileChecker.hpp"
#include "Logger.hpp"

int main(int argc, char *argv[]) {

    try
    {
        if(argc < 2)
            throw FileChecker::NoFileException();

        Taskmaster taskmaster(argv[1]);
    }
    catch (std::exception& e)
    {
        Logger::error(e.what());
    }
    
    return 0;

}
