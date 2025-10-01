#pragma once

#include <signal.h>
#include <string>
#include <unordered_map>

#include "Process.hpp"

class SigUtils {

    private:
        static volatile sig_atomic_t _receivedSIGHUP;

    public:
        static bool        getReceivedSIGHUP( void );
        static void        setReceivedSIGHUP( bool receivedSIGHUP );

        static int  strToSignal(const std::string& sigStr);
        static bool isStopSignal(const Process& process, int exitCode);
        static void        handleSIGHUP( int );
};
