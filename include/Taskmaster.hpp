#pragma once

#include "Config.hpp"
#include "ProcessManager.hpp"
#include "Shell.hpp"
#include "Logger.hpp"


class Taskmaster {

    private:
        Config                          _config;
        std::unique_ptr<ProcessManager>  _process_manager;
        Shell                           _shell;
        std::string                     _configFile;

    public:
        Taskmaster(const std::string& configFile);
        ~Taskmaster();

};
