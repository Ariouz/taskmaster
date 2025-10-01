#include "SigUtils.hpp"

volatile sig_atomic_t SigUtils::_receivedSIGHUP = false;


int SigUtils::strToSignal(const std::string& sigStr) {
    static const std::unordered_map<std::string, int> signals = {
        {"HUP", SIGHUP},
        {"INT", SIGINT},
        {"TERM", SIGTERM},
        {"KILL", SIGKILL},
        {"USR1", SIGUSR1},
        {"USR2", SIGUSR2}
    };

    auto it = signals.find(sigStr);
    if (it != signals.end()) return it->second;
    return SIGTERM;
}

bool SigUtils::isStopSignal(const Process& process, int exitCode) {
    return exitCode == SigUtils::strToSignal(process.getConfig().getStopsignal());
}

void SigUtils::handleSIGHUP( int ) {
    _receivedSIGHUP = true;
}

bool    SigUtils::getReceivedSIGHUP( void ) {
    return _receivedSIGHUP;
}

void    SigUtils::setReceivedSIGHUP( bool receivedSIGHUP ) {
    _receivedSIGHUP = receivedSIGHUP;
}
