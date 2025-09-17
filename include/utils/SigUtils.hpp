#include <signal.h>
#include <string>
#include <unordered_map>

#include "Process.hpp"

class SigUtils {

    public:
        static int strToSignal(const std::string& sigStr);
        static bool isStopSignal(const Process& process, int exitCode);
};