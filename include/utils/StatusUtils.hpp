#pragma once
#include <string>
#include "Status.hpp"

class StatusUtils {
public:
    static std::string toString(Status s) {
        switch (s) {
            case Status::STARTING: return "STARTING";
            case Status::RUNNING:  return "RUNNING";
            case Status::STOPPED:  return "STOPPED";
            case Status::EXITED:   return "EXITED";
            case Status::FATAL:    return "FATAL";
            default:               return "UNKNOWN";
        }
    }
};
