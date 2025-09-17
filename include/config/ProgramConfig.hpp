#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

struct ProgramConfig {
    std::string name;
    std::string cmd;
    int numprocs = 1;
    bool autostart = false;
    enum class AutoRestart { ALWAYS, NEVER, UNEXPECTED } autorestart;
    std::vector<int> exitcodes;
    int startretries = 3;
    int starttime = 1;
    std::string stopsignal = "TERM";
    int stoptime = 10;
    std::string stdout_file;
    std::string stderr_file;
    std::map<std::string, std::string> env;
    std::string workingdir = ".";
    mode_t umask = 000;
};
