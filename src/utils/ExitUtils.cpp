#include "ExitUtils.hpp"
#include "Process.hpp"
#include <algorithm>

bool ExitUtils::isExpectedExit(const Process& process, int exitCode) {
    return std::find(process.getConfig().getExitcodes().begin(), process.getConfig().getExitcodes().end(), exitCode) != process.getConfig().getExitcodes().end();
}
