#include "ProcessManager.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include "Logger.hpp"

ProcessManager::ProcessManager() = default;
ProcessManager::~ProcessManager() = default;

///////////---  TEMP METHOD ---///////////
void ProcessManager::init() {
    ProgramConfig cfg;
    cfg.setProgramName("Echo");
    cfg.setCmd("echo test");
    cfg.setAutostart(true);

    if (!cfg.getAutostart()) return;

    Process process = createProcess(cfg);
    if (process.getPid() == -1) return;

    this->_processes.emplace(process.getPid(), process);
}


Process ProcessManager::createProcess(const ProgramConfig& cfg) {
    int pid = fork();

    if (pid == -1) {
        Logger::error("Failed to fork process " + cfg.getProgramName());
        return Process(pid, Status::FATAL, cfg);
    }

    if (pid == 0) { // child
        execlp("sh", "sh", "-c", cfg.getCmd().c_str(), nullptr);
        _exit(EXIT_FAILURE);
    }

    else { // parent
        Logger::info("Started process " + cfg.getProgramName() + " with pid " + std::to_string(pid));
        Process process = Process(pid, Status::STARTING, cfg);
        return process;
    }
}

/*
    std::string cmd;
    int numprocs = 1;
    bool autostart = false;
    enum class AutoRestart { ALWAYS, NEVER, UNEXPECTED } autorestart;
    std::vector<int> exitcodes;
    int startretries = 3;
    int starttime = 1;
    std::string stopsignal = "TERM";
    int stoptime = 10;
    std::string stdout_file;
    std::string stderr_file;
    std::map<std::string, std::string> env;
    std::string workingdir = ".";
    mode_t umask = 022;
*/
