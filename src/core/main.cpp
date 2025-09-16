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
    catch (Taskmaster::NoFileException& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (Taskmaster::NotRegularFileException& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (Taskmaster::ReadableFileException& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;

}
