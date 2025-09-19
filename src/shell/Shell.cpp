#include "Shell.hpp"

std::vector<std::string> Shell::_commands = { "run", "status", "start", "stop", "reload", "quit" };
std::map<std::string, std::function<void()>> Shell::_commands_functions;


////////// Constructors | Destructor ////////// 

Shell::Shell() {
    _commands_functions = {
        { "run",    [this]() { run(); } },
        { "status", [this]() { status(); } },
        { "start",  [this]() { start(); } },
        { "stop",   [this]() { stop(); } },
        { "reload", [this]() { reload(); } },
        { "quit",   [this]() { quit(); } }
    };

    rl_attempted_completion_function = &Shell::custom_completion;

    while ((this->_readline_return = readline("\033[38;5;154mtaskmaster> \033[0m"))) {
        if (this->_readline_return && *this->_readline_return)
            add_history(this->_readline_return);

        if (this->_readline_return) {
            std::string command = this->_readline_return;

            this->functionsCall(command);

            free(this->_readline_return);
        }
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

void Shell::functionsCall( const std::string readline_return ) const {
        std::istringstream iss(readline_return);

        std::string command;
        iss >> command;

        try
        {
            _commands_functions[command]();
        }
        catch (std::exception& e)
        {
            std::cerr << "*** Unknown syntax: " << command << std::endl;
        }
}

void    Shell::run( void ) {
    std::cout << "run" << std::endl;
}

void    Shell::status( void ) {
    std::cout << "status" << std::endl;
}

void    Shell::start( void ) {
    std::cout << "start" << std::endl;
}

void    Shell::stop( void ) {
    std::cout << "stop" << std::endl;
}

void    Shell::reload( void ) {
    std::cout << "reload" << std::endl;
}

void    Shell::quit( void ) {
    std::cout << "quit" << std::endl;
}
