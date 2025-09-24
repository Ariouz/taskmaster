#include "Status.hpp"
#include "Logger.hpp"

std::ostream& operator<<(std::ostream& os, Status status) {
    switch (status) {
        #ifndef BONUS
            case Status::STARTING: os << "STARTING"; break;
            case Status::RUNNING:  os << "RUNNING"; break;
            case Status::STOPPED:  os << "STOPPED"; break;
            case Status::EXITED:   os << "EXITED"; break;
            case Status::FATAL:    os << "FATAL"; break;
            default:               os << "UNKNOWN"; break;
        #else
            case Status::STARTING: os << std::string(COLOR_INFO) + "STARTING" + COLOR_RESET; break;
            case Status::RUNNING:  os << std::string(COLOR_SUCCESS) + "RUNNING" + COLOR_RESET; break;
            case Status::STOPPED:  os << std::string(COLOR_WARN) + "STOPPED" + COLOR_RESET; break;
            case Status::EXITED:   os << std::string(COLOR_WARN) + "EXITED" + COLOR_RESET; break;
            case Status::FATAL:    os << std::string(COLOR_ERROR) + "FATAL" + COLOR_RESET; break;
            default:               os << std::string(COLOR_UNKNOWN) + "UNKNOWN" + COLOR_RESET; break;
        #endif
    }
    return os;
}
