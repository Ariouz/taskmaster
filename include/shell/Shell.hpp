#pragma once

#include <string>
#include <readline/readline.h>
#include <readline/history.h>
#include <vector>

class Shell {

    private:
        char*                           _readline_return;
        static std::vector<std::string> _commands;

    public:
        Shell();
        ~Shell();

        void run();
        void status();
        void start(std::string name);
        void stop(std::string name);
        void reload();
        void quit();

        static char*   command_generator(const char* text, int state);
        static char**  custom_completion(const char* text, int start, int end);

};
