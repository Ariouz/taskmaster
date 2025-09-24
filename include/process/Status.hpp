#pragma once

#include <iostream>

enum class Status {
    STARTING,
    RUNNING,
    STOPPING,
    STOPPED,
    EXITED,
    FATAL,
    UNKNOWN
};

std::ostream& operator<<(std::ostream& os, Status status);
