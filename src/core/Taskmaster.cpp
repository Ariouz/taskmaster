#include "Taskmaster.hpp"
#include "FileChecker.hpp"
#include "Config.hpp"
#include "Shell.hpp"
#include <thread>
#include <csignal>

Taskmaster* Taskmaster::_instance = nullptr;
std::atomic<bool>       logs_mode_flag {false};


void Taskmaster::sigHandler(int sig) {
    if (sig == SIGINT) {
        if (logs_mode_flag.load()) logs_mode_flag.store(false);
        else std::cout << std::endl << "Please use quit" << std::endl << "\033[38;5;154mtaskmaster> \033[0m";
    }
}

Taskmaster::Taskmaster(const std::string& configFile) {
    _instance = this;
    FileChecker::checkFile(configFile);
    this->_config = Config(configFile);
    Logger::info("Loaded taskmaster with config file " + configFile);

    std::signal(SIGINT, Taskmaster::sigHandler);

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
