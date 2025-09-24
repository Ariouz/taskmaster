#include "Process.hpp"
#include "SigUtils.hpp"
#include "ExitUtils.hpp"
#include <algorithm>

Process::Process() = default;
Process::Process(pid_t pid, Status status, ProgramConfig config) 
: _name(config.getProgramName()), _pid(pid), _status(status), _start_time(std::time(nullptr)), _config(config) {}

Process::~Process() = default;

std::string Process::getName() const {
    return this->_name;
}

int Process::getPid() const {
    return this->_pid;
}

int Process::getRetries() const {
    return this->_retries;
}

Status Process::getStatus() const {
    return this->_status;
}

ProgramConfig  Process::getConfig() const {
    return this->_config;
}

time_t    Process::getStartTime() const {
    return this->_start_time;
}

void    Process::setStatus(Status status) {
    this->_status = status;
}

void    Process::setRetries(int retries) {
    this->_retries = retries;
}

bool  Process::shouldRestart(int exitCode, bool killedBySignal = false) const {
    if (this->_config.getAutorestart() == AutoRestart::NEVER) return false;
    if (this->_retries >= this->_config.getStartretries()) return false;


    if (this->_config.getAutorestart() == AutoRestart::UNEXPECTED) {

        if (killedBySignal && SigUtils::isStopSignal(*this, exitCode)) return false;
        if (ExitUtils::isExpectedExit(*this, exitCode)) return false;
    }

    return true;
}

std::string Process::uptimeStr( void ) const {
    time_t now = std::time(nullptr);
    time_t time = now - this->_start_time;

    int hours = time / 3600;
    int minutes = (time % 3600) / 60;
    int seconds = time % 60;

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hours << ":"
        << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << seconds;

    return oss.str();
}
