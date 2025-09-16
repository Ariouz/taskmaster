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

        void start(ProgramConfig& config);
        void stop(std::string name);
        void restart(std::string name);
        void monitor();
        void handleSigchld(int sig);

};
