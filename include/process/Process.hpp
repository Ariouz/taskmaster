#include "Status.hpp"
#include <sys/types.h>
#include <ctime>

class Process {

    private:
        pid_t   _pid;
        Status  _status;
        int     _retries;
        time_t  _start_time;

    public:
        Process(pid_t pid, Status status);
        ~Process();

        bool isAlive();
        void kill(int sigcode);
};