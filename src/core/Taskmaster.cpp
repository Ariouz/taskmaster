#include "Taskmaster.hpp"
#include "FileChecker.hpp"
#include "Config.hpp"
#include "Shell.hpp"
#include <thread>

Taskmaster::Taskmaster(const std::string& configFile) {
    FileChecker::checkFile(configFile);
    this->_config = Config(configFile);
    Logger::info("Loaded taskmaster with config file " + configFile);


    _process_manager = std::make_unique<ProcessManager>(_config);
    _process_manager->init();

    std::thread monitorThread([this]() {
        while (true) {
           this->_process_manager->monitor();
           std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    monitorThread.detach();
    this->_shell = Shell(_process_manager.get());
}

Taskmaster::~Taskmaster() = default;
