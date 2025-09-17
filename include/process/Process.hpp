#pragma once

#include "Status.hpp"
#include <sys/types.h>
#include <ctime>
#include "ProgramConfig.hpp"

class Process {

    private:
        std::string     _name;
        pid_t           _pid;
        Status          _status = Status::STARTING;
        int             _retries = 0;
        time_t          _start_time;
        ProgramConfig   _config;

    public:
        Process();
        Process(pid_t pid, Status status, ProgramConfig config);
        ~Process();

        bool isAlive();
        void kill(int sigcode);

        std::string       getName() const;
        int               getPid() const;
        int               getRetries() const;
        Status            getStatus() const;
        ProgramConfig     getConfig() const;
        time_t            getStartTime() const;

        void              setStatus(Status status);
        void              setRetries(int retries);

        bool              shouldRestart(int exitCode, bool killedBySignal) const;
};
