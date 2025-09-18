#include "Shell.hpp"

std::vector<std::string> Shell::_commands = { "run", "status", "start", "stop", "reload", "quit" };


////////// Constructors | Destructor ////////// 

Shell::Shell() {
    rl_attempted_completion_function = &Shell::custom_completion;

    while ((this->_readline_return = readline("\033[38;5;154mtaskmaster> \033[0m"))) {
        if (this->_readline_return && *this->_readline_return)
            add_history(this->_readline_return);

        //Commande a implementer

        free(this->_readline_return);
    }
}

Shell::~Shell() = default;


////////// Functions ////////// 

char* Shell::command_generator(const char* text, int state) {
    static size_t list_index;
    static size_t len;

    if (state == 0) {
        list_index = 0;
        len = strlen(text);
    }

    while (list_index < Shell::_commands.size()) {
        const std::string& cmd = Shell::_commands[list_index++];
        if (cmd.compare(0, len, text) == 0)
            return strdup(cmd.c_str());
    }

    return nullptr;
}

char** Shell::custom_completion(const char* text, int start, int end) {
    (void)start;
    (void)end;

    rl_attempted_completion_over = 1;

    return rl_completion_matches(text, &Shell::command_generator);
}
