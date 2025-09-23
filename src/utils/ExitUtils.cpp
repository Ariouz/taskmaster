#include "ExitUtils.hpp"
#include "Process.hpp"
#include <algorithm>

bool ExitUtils::isExpectedExit(const Process& process, int exitCode) {
    const auto& exits = process.getConfig().getExitcodes();
    return std::find(exits.begin(), exits.end(), exitCode) != exits.end();
}

