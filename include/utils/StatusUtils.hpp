#pragma once
#include <string>
#include "Status.hpp"
#include "Logger.hpp"

class StatusUtils {
public:
    static std::string toString(Status s) {
        switch (s) {
            #ifndef BONUS
            case Status::STARTING: return "STARTING";
            case Status::RUNNING:  return "RUNNING";
            case Status::STOPPED:  return "STOPPED";
            case Status::EXITED:   return "EXITED";
            case Status::FATAL:    return "FATAL";
            default:               return "UNKNOWN";
            #else
            case Status::STARTING: return std::string(COLOR_INFO) + "STARTING" + COLOR_RESET;
            case Status::RUNNING:  return std::string(COLOR_SUCCESS) + "RUNNING" + COLOR_RESET;
            case Status::STOPPED:  return std::string(COLOR_WARN) + "STOPPED" + COLOR_RESET;
            case Status::EXITED:   return std::string(COLOR_WARN) + "EXITED" + COLOR_RESET;
            case Status::FATAL:    return std::string(COLOR_ERROR) + "FATAL" + COLOR_RESET;
            default:               return std::string(COLOR_UNKNOWN) + "UNKNOWN" + COLOR_RESET;
            #endif
        }
    }
};
