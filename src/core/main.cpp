#include <stdexcept>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "Taskmaster.hpp"

int main(int argc, char *argv[]) {

    try
    {
        if(argc < 2)
            throw Taskmaster::NoFileException();

        Taskmaster taskmaster(argv[1]);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;

}
