#pragma once

#include <map>
#include <mutex>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Process.hpp"
#include "Config.hpp"

class ProcessManager {
    private:
        Config&                 _config;
        std::map<int, Process>  _processes;
        std::mutex              _mtx;

    public:
        ProcessManager(Config& config);
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

        void                redirectOutputs(const ProgramConfig& cfg);
        std::vector<char*>  prepareEnv(const ProgramConfig& cfg);

        void                    setConfig(const Config& config);
        
        std::mutex&             getMutex();
        Config&                 getConfig() const;
        std::map<int, Process>& getProcesses();

};
