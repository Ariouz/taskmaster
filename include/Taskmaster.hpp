#include "Config.hpp"
#include "ProcessManager.hpp"
#include "Shell.hpp"
#include "Logger.hpp"

class Taskmaster {

    private:
        Config          _config;
        ProcessManager  _process_manager;
        Shell           _shell;
        Logger          _logger;

    public:
        Taskmaster(const std::string& configFile);
        ~Taskmaster();
};