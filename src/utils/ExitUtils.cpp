#include "ExitUtils.hpp"
#include <algorithm>

bool ExitUtils::isExpectedExit(const Process& process, int exitCode) {
    return std::find(process.getConfig().exitcodes.begin(), process.getConfig().exitcodes.end(), exitCode) != process.getConfig().exitcodes.end();
}