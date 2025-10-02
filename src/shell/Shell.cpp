#include "Shell.hpp"

std::vector<std::string> Shell::_commands = { "run", "status", "start", "stop", "reload", "quit",
    #ifdef BONUS
        "logs"
    #endif 
};

std::vector<std::string> Shell::_programs_list;
const std::vector<std::string>* Shell::_current_completion_list = &Shell::_commands;
std::map<std::string, std::function<void(const std::string& arg)>> Shell::_commands_functions;

////////// Constructors | Destructor ////////// 

Shell::Shell(const std::string& configFile, ProcessManager* pm) : _pm(pm), _configFile(configFile) {
    this->init_commands_functions_map();

    this->init_programs_list_vector();

    rl_attempted_completion_function = &Shell::custom_completion;

    this->readline_util();
}

Shell::~Shell() = default;


////////// Functions ////////// 

std::vector<std::string>&    Shell::get_programs_list( void ) {
    return _programs_list;
}

std::vector<std::string>    Shell::_initArgs( const std::string& arg ) {
    std::vector<std::string> args;

    if (arg.empty()) {
        for (auto& program : _pm->getConfig().getPrograms())
            args.push_back(program.second.getProgramName());
    } else {
        args = split_multi_delim(arg, " \t");
    }

    return  args;
}

void	Shell::set_termios_handle(void) {
	struct termios	termios_p;

	tcgetattr(STDIN_FILENO, &termios_p);
	termios_p.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &termios_p);
}

void	Shell::sigint_handler(int signum) {
	(void) signum;

    if (logs_mode_flag.load()) logs_mode_flag.store(false);
    else {
        printf("\n");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void	Shell::signal(void) {
	struct sigaction	action;

	action.sa_handler = Shell::sigint_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);
}

char* Shell::command_generator(const char* text, int state) {
    static size_t list_index;
    static size_t len;

    if (state == 0) {
        list_index = 0;
        len = strlen(text);
    }

    while (list_index < _current_completion_list->size()) {
        const std::string& cmd = (*_current_completion_list)[list_index++];
        if (cmd.compare(0, len, text) == 0)
            return strdup(cmd.c_str());
    }

    return nullptr;
}

char** Shell::custom_completion(const char* text, int start, int end) {
    (void)start;
    (void)end;

    rl_attempted_completion_over = 1;

    std::string line(rl_line_buffer);
    line.erase(0, line.find_first_not_of(" \t"));

    std::istringstream iss(line);
    std::string first_word;
    iss >> first_word;

    static const std::set<std::string> commands_with_args = {
        "run", "start", "stop", "reload", "status"
    };

    if (commands_with_args.count(first_word) && line.size() > first_word.size() && std::isspace(line[first_word.size()])) {
        _current_completion_list = &_programs_list;
    } else {
        _current_completion_list = &_commands;
    }

    return rl_completion_matches(text, &Shell::command_generator);
}

std::vector<std::string> Shell::split_multi_delim(const std::string& str, const std::string& delimiters) {
    std::vector<std::string> tokens;
    std::string current;

    for (char ch : str) {
        if (delimiters.find(ch) != std::string::npos) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        } else {
            current += ch;
        }
    }

    if (!current.empty())
        tokens.push_back(current);

    return tokens;
}

void Shell::functionsCall( const std::string readline_return ) const {
        std::istringstream iss(readline_return);

        std::string command;
        iss >> command;

        if (command.empty()) return ;

        std::string arg;
        std::getline(iss, arg);
        if (!arg.empty() && arg[0] == ' ')
            arg.erase(0, 1);

        try {
            _commands_functions[command](arg);
        } catch (std::exception& e) {
            std::cerr << "*** Unknown syntax: " << command << std::endl;
        }
}

void    Shell::init_commands_functions_map( void ) {
    this->_commands_functions = {
        { "status", [this](const std::string& arg) { status(arg); } },
        { "start",  [this](const std::string& arg) { start(arg); } },
        { "stop",   [this](const std::string& arg) { stop(arg); } },
        { "reload", [this](const std::string& arg) { (void) arg; reload(); } },
        { "quit",   [this](const std::string& arg) { (void) arg; quit(); } }
        #ifdef BONUS
        ,{ "logs",   [this](const std::string& arg) { (void) arg; logs(); } }
        #endif
    };
}

void    Shell::init_programs_list_vector( void ) {
    std::map<std::string, ProgramConfig> programs = this->_pm->getConfig().getPrograms();

    for (std::map<std::string, ProgramConfig>::iterator it=programs.begin(); it!=programs.end(); ++it)
        this->_programs_list.push_back(it->first);
}

void    Shell::readline_util( void ) {
    set_termios_handle();
    signal();
    while (1) {
        this->_readline_return = readline("\033[38;5;154mtaskmaster> \033[0m");

        if (SigUtils::getReceivedSIGHUP()) {
            SigUtils::setReceivedSIGHUP(false);
            this->reload();
            continue;
        }

        if (!this->_readline_return)
            this->quit();

        if (this->_readline_return && *this->_readline_return)
            add_history(this->_readline_return);

        if (this->_readline_return) {
            std::string command = this->_readline_return;

            this->functionsCall(command);

            free(this->_readline_return);
        }
    }
}

void    Shell::status( const std::string& arg ) {
    std::lock_guard<std::mutex> lock(_pm->getMutex());
    std::vector<std::string> args = this->_initArgs(arg);
    std::string str = "";

    for (auto& program : args) {
        int count = 0;

        for (auto& [pid, proc] : _pm->getProcesses()) {
            if (proc.getName() == program) {
                str = proc.getName() + std::string("[") + std::to_string(count)
                        + std::string("]: pid=") + std::to_string(proc.getPid())
                        + std::string(" status=") + proc.statusToString(proc.getStatus());

                if (proc.getStatus() == Status::RUNNING) {
                    str += std::string(" uptime=") + proc.uptimeStr();
                }

                std::cout << str << std::endl;
                Logger::info(str);

                count++;
            }
        }

        for (; count < _pm->getConfig().getPrograms()[program].getNumprocs(); ++count) {
            str = program + std::string(": ERROR (no such process)");
            std::cerr << str << std::endl;
            Logger::error(str);
        }
    }
    
}

void    Shell::start( const std::string& arg ) {
    if (arg.empty()) {
        std::cerr << "Syntax: start <progam>" << std::endl;
        return ;
    }

    const auto& allPrograms = _pm->getConfig().getPrograms();
    auto it = allPrograms.find(arg);
    if (it == allPrograms.end()) {
        std::cerr << "Unknown program: " << arg << std::endl;
        return;
    }

    const ProgramConfig& cfg = it->second;
    for (int i = 0; i < cfg.getNumprocs(); i++) {
        Process proc = _pm->createProcess(cfg);
        if (proc.getPid() == -1) {
            Logger::error("Failed to start process " + cfg.getProgramName());
            continue;
        }

        std::lock_guard<std::mutex> lock(_pm->getMutex());
        _pm->getProcesses().emplace(proc.getPid(), proc);
        Logger::info("Started process " + cfg.getProgramName() + " with pid " + std::to_string(proc.getPid()));
    }   

}

void    Shell::stop( const std::string& arg ) {
    std::lock_guard<std::mutex> lock(_pm->getMutex());

    std::string str = "";
    std::vector<std::string> args = this->_initArgs(arg);

    for (auto& program : args) {
        int count = 0;

        for (auto& [pid, proc] : _pm->getProcesses()) {
            if (proc.getName() == program) {
                str = proc.getName() + std::string("[") + std::to_string(count) + std::string("]: ");

                std::string sigstr = _pm->getConfig().getPrograms()[program].getStopsignal();
                int signal = _pm->getConfig().getPrograms()[program].getSig()[sigstr];

                if (kill(proc.getPid(), signal) == 0) {
                    str += "stopped";
                    std::cout << str << std::endl;
                    Logger::info(str);
                } else {
                    str += "ERROR (not running)";
                    std::cerr << str << std::endl;
                    Logger::error(str);
                }

                count++;
            }
        }

        for (; count < _pm->getConfig().getPrograms()[program].getNumprocs(); ++count) {
            std::cout << program
            << ": ERROR (no such process)"
            << std::endl;
        }
    }
}

void    Shell::reload( void ) {
    FileChecker::checkFile(this->_configFile);
    this->_temp_config_reload = Config(this->_configFile);

    std::lock_guard<std::mutex> lock(_pm->getMutex());

    FileChecker::yamlComparator(*this->_pm, this->_temp_config_reload.getPrograms());
    
}

void    Shell::quit( void ) {
    std::string pid;
    std::streambuf* cout_buf = std::cout.rdbuf();
    std::ofstream null_stream("/dev/null");
    std::cout.rdbuf(null_stream.rdbuf());

    this->stop("");

    std::cout.rdbuf(cout_buf);

    for (const auto& pair : _pm->getProcesses()) {
        pid = std::to_string(pair.first);
        if (kill(pair.first, SIGKILL) == 0) {
            Logger::info("pid " + pid + " killed");
        }
    }

    free(this->_readline_return);

    std::lock_guard<std::mutex> lock(_pm->getMutex());
    exit (0);
}

#ifdef BONUS
void    Shell::logs() {
    logs_mode_flag.store(true);
    Logger::setVerbose(true);
    while (logs_mode_flag.load())
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Logger::setVerbose(false);
}
#endif
