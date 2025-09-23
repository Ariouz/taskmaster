#include "ProcessManager.hpp"
#include "Logger.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>
#include <cstring>

ProcessManager::ProcessManager(Config& config) : _config(config) {}
ProcessManager::~ProcessManager() = default;

void ProcessManager::init() {
    for(const auto& [name, progConf] : _config.getPrograms()) {
        if (!progConf.getAutostart()) continue;;
        
        for (int i = 0; i < progConf.getNumprocs(); i++) {
            Process process = createProcess(progConf);
            if (process.getPid() == -1) continue;
            
            std::lock_guard<std::mutex> lock(_mtx);
            this->_processes.emplace(process.getPid(), process);
        }
    }
}

std::vector<char*> splitCommand(const std::string& cmd) {
    std::istringstream iss(cmd);
    std::vector<std::string> parts;
    std::string token;
    while (iss >> token) {
        parts.push_back(token);
    }

    std::vector<char*> argv;
    for (auto& p : parts)
        argv.push_back(const_cast<char*>(p.c_str()));
    argv.push_back(nullptr);
    return argv;
}


Process ProcessManager::createProcess(const ProgramConfig& cfg) {
    int pid = fork();
    
    if (pid == -1) {
        Logger::error("Failed to fork process " + cfg.getProgramName());
        return Process(pid, Status::FATAL, cfg);
    }
    
    if (pid == 0) { // child
        setpgid(0, 0);
        umask(cfg.getUmask());
        redirectOutputs(cfg);
        if (chdir(cfg.getWorkingdir().c_str()) == -1) {
            std::cerr << "[ERROR] Failed to set working dir to " << cfg.getWorkingdir() << std::endl;
            _exit(EXIT_FAILURE);
        }
        
        // ENV
        std::vector<char*> env = prepareEnv(cfg);
        for (char **e = env.data(); *e != nullptr; ++e)
            std::cerr << "[DEBUG] ENV: " << *e << std::endl;

        // CMD
        std::vector<char*> argv = splitCommand(cfg.getCmd());
        std::cerr << "[DEBUG] execvpe " << argv[0] << std::endl;

        if (execvpe(argv[0], argv.data(), env.data()) == -1) {
            perror("execvpe failed");
             _exit(EXIT_FAILURE);
        }

        // NEVER REACHED
        return Process(-1, Status::FATAL, cfg);
    }
    
    else { // parent
        Logger::info("Started process " + cfg.getProgramName() + " with pid " + std::to_string(pid));
        Process process = Process(pid, Status::STARTING, cfg);
        return process;
    }
}

extern char **environ;
std::vector<char*> ProcessManager::prepareEnv(const ProgramConfig& cfg) {
    std::vector<char*> env;
    std::vector<std::string> env_strings;
    for (char **e = environ; *e != nullptr; ++e)
    env_strings.emplace_back(*e);
    
    for (auto& kv : cfg.getEnv()) {
        bool found = false;
        for (auto& s : env_strings) {
            if (s.find(kv.first + "=") == 0) {
                s = kv.first + "=" + kv.second;
                found = true;
                break;
            }
        }
        if (!found)
        env_strings.push_back(kv.first + "=" + kv.second);
    }
    
    for (auto& s : env_strings)
        env.push_back(strdup(s.c_str()));
    env.push_back(nullptr);
    return env;
}


void ProcessManager::redirectOutputs(const ProgramConfig& cfg) {
    auto redirect = [](const std::string& path, int fd) {
        int target = path.empty() ? open("/dev/null", O_WRONLY) 
        : open(path.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (target != -1) {
            dup2(target, fd);
            close(target);
        }
    };
    
    redirect(cfg.getStdoutFile(), STDOUT_FILENO);
    redirect(cfg.getStderrFile(), STDERR_FILENO);
}


void    ProcessManager::setConfig(const Config& config) {
    _config = config;
}

std::mutex& ProcessManager::getMutex() {
    return _mtx;
}

Config& ProcessManager::getConfig() const {
    return _config;
}

std::map<int, Process>& ProcessManager::getProcesses() {
    return _processes;
}