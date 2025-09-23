#include "Shell.hpp"
#include "Logger.hpp"
#include "StatusUtils.hpp"
#include <thread>

std::vector<std::string> Shell::_commands = { "run", "status", "start", "stop", "reload", "quit",
    #ifdef BONUS
        "logs"
    #endif 
};
std::map<std::string, std::function<void(const std::string& arg)>> Shell::_commands_functions;


////////// Constructors | Destructor ////////// 

Shell::Shell(ProcessManager* pm) : _pm(pm) {
    _commands_functions = {
        // { "run",    [this](const std::string& arg) { run(); } },
        { "status", [this](const std::string& arg) { (void) arg; status(); } },
        { "start",  [this](const std::string& arg) { start(arg); } },
        { "stop",   [this](const std::string& arg) { stop(arg); } },
        { "reload", [this](const std::string& arg) { (void) arg; reload(); } },
        { "quit",   [this](const std::string& arg) { (void) arg; quit(); } }
        #ifdef BONUS
        ,{ "logs",   [this](const std::string& arg) { (void) arg; logs(); } }
        #endif
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

        if (command.empty()) return ;

        std::string arg;
        std::getline(iss, arg);
        if (!arg.empty() && arg[0] == ' ')
            arg.erase(0, 1);

        try
        {
            _commands_functions[command](arg);
        }
        catch (std::exception& e)
        {
            std::cerr << "*** Unknown syntax: " << command << std::endl;
        }
}

// void    Shell::run( void ) {
//     std::cout << "run" << std::endl;
// }

void    Shell::status() {
    std::cout << "status" << std::endl;
    std::lock_guard<std::mutex> lock(_pm->getMutex());
    for (auto& kv : _pm->getConfig().getPrograms()) {
        const ProgramConfig& cfg = kv.second;
        int count = 0;
        for (auto& [pid, proc] : _pm->getProcesses()) {
            if (proc.getName() == cfg.getProgramName()) {
                std::cout << cfg.getProgramName()
                << "[" << count << "]: pid=" << proc.getPid()
                << " status=" << StatusUtils::toString(proc.getStatus())
                << std::endl;
                count++;
            }
        }
        
        for (; count < cfg.getNumprocs(); ++count) {
            std::cout << cfg.getProgramName()
            << "[" << count << "]: not running"
            << std::endl;
        }
    }
    
}

void    Shell::start( const std::string& arg ) {
    std::cout << "start " << arg << std::endl;

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
    (void) arg;
    std::cout << "stop" << arg << std::endl;
    std::lock_guard<std::mutex> lock(_pm->getMutex());
}

void    Shell::reload() {
    std::cout << "reload" << std::endl;
    std::lock_guard<std::mutex> lock(_pm->getMutex());
}

void    Shell::quit() {
    std::cout << "quit" << std::endl;
    std::lock_guard<std::mutex> lock(_pm->getMutex());
    // stop processes + exit
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