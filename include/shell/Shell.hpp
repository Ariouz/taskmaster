#pragma once

#include <string>
#include <readline/readline.h>

class Shell {

    private:
        std::string readline(std::string command);

    public:
        Shell();
        ~Shell();

        void run();
        void status();
        void start(std::string name);
        void stop(std::string name);
        void reload();
        void quit();

};
