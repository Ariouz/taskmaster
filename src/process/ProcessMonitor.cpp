#include "ProcessManager.hpp"
#include "Logger.hpp"
#include "sys/wait.h"
#include "ExitUtils.hpp"
#include "SigUtils.hpp"

void ProcessManager::monitor() {
    std::lock_guard<std::mutex> lock(_mtx);
    for (auto it = _processes.begin(); it != _processes.end();) {
        int pid = it->first;
        Process& process = it->second;

        if (process.getStatus() == Status::EXITED
            || process.getStatus() == Status::FATAL
            ||process.getStatus() == Status::STOPPED) {
            
            it++;
            continue;
        }

        int status;
        pid_t result = waitpid(pid, &status, WNOHANG);

        if (result == 0) {
            time_t now = std::time(NULL);
            time_t diff = now - process.getStartTime();
            if (process.getStatus() == Status::STARTING && diff >= process.getConfig().getStarttime()) process.setStatus(Status::RUNNING);
            it++;
            continue;
        }

        if (result == -1) {
            Logger::error("Error while waiting for pid " + std::to_string(pid));
            it++;
            continue;
        }

        if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);

            if (ExitUtils::isExpectedExit(process, code)) {
                process.setStatus(Status::EXITED); 
                Logger::info("Process " + process.getName() + " exited with code " + std::to_string(code));
            }
            else {
                process.setStatus(Status::FATAL); 
                Logger::warn("Process " + process.getName() + " exited with unexpected code " + std::to_string(code));
            }

            handleAutoRestart(process, code);
        }
        else if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            
            if (SigUtils::isStopSignal(process, sig)) {
                process.setStatus(Status::STOPPED);
                Logger::info("Process " + process.getName() + " killed by signal " + std::to_string(sig));
                // No restart if stopped using stop command
            }
            else {
                process.setStatus(Status::FATAL);
                Logger::warn("Process " + process.getName() + " killed by unexpected signal " + std::to_string(sig));
                handleAutoRestart(process, sig, true);
            }
        }

    }
}

void ProcessManager::handleAutoRestart(Process& process, int code, bool killedBySignal) {
    if (process.shouldRestart(code, killedBySignal)) {
        Process new_proc = createProcess(process.getConfig());
        new_proc.setRetries(process.getRetries() + 1);
        Logger::info("Restarting " + new_proc.getName() + ": " + std::to_string(new_proc.getRetries()) + "/" + std::to_string(new_proc.getConfig().getStartretries()));

        _processes[new_proc.getPid()] = new_proc;
    }
}
