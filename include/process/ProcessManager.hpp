#pragma once

#include <map>
#include "Process.hpp"
#include "ProgramConfig.hpp"

class ProcessManager {
    private:
        std::map<int, Process> _processes;

    public:
        ProcessManager();
        ~ProcessManager();

        // temp methods
        void        init();
        Process     createProcess(const ProgramConfig& cfg);

        void monitor();
        void handleAutoRestart(Process& process, int code, bool killedBySignal = false);

        void start(ProgramConfig& config);
        void stop(std::string name);
        void restart(std::string name);
        void handleSigchld(int sig);

};
