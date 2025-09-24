#include "Status.hpp"

std::ostream& operator<<(std::ostream& os, Status status) {
    switch (status) {
        case Status::STARTING: os << "STARTING"; break;
        case Status::RUNNING: os << "RUNNING"; break;
        case Status::STOPPING: os << "STOPPING"; break;
        case Status::STOPPED: os << "STOPPED"; break;
        case Status::EXITED: os << "EXITED"; break;
        case Status::FATAL: os << "FATAL"; break;
        case Status::UNKNOWN: os << "UNKNOWN"; break;
    }
    return os;
}
