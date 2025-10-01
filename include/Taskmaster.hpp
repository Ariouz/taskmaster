#pragma once

#include "Config.hpp"
#include "ProcessManager.hpp"
#include "Shell.hpp"
#include "Logger.hpp"
#include "FileChecker.hpp"
#include "SigUtils.hpp"
#include <thread>
#include <csignal>


class Taskmaster {

    private:
        Config                          _config;
        std::unique_ptr<ProcessManager>  _process_manager;
        Shell                           _shell;
        std::string                     _configFile;

        static void                     sigHandler(int sig);

    public:
        Taskmaster(const std::string& configFile);
        ~Taskmaster();

};
