#include <signal.h>
#include <string>
#include <unordered_map>

#include "Process.hpp"

class ExitUtils {

    public:
        static bool isExpectedExit(const Process& process, int exitCode);

};