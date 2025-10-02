#include "FileChecker.hpp"

void    FileChecker::checkFile( const std::string& configFile ) {
    std::filesystem::path filePath(configFile);

    if (!std::filesystem::is_regular_file(filePath)) {
        throw NotRegularFileException();
    }

    if (!(filePath.extension() == ".yaml" || filePath.extension() == ".yml"))
        throw NoFileException();

    if (!std::filesystem::exists(filePath))
        throw NoFileException();

    std::ifstream file(filePath);
    if (!file.good())
        throw ReadableFileException();
    
}

void FileChecker::yamlComparator( ProcessManager& pm, std::map<std::string, ProgramConfig> toCompare ) {

    std::set<std::string> commonKeys, baseKeys, toCompareKeys;
    std::map<std::string, ProgramConfig> base = pm.getConfig().getPrograms();

    for (const auto& [key, _] : base)
        if (toCompare.find(key) != toCompare.end())
            commonKeys.insert(key);

    if (!commonKeys.empty())
        yamlComparatorElements(commonKeys, base, toCompare);

    for (const auto& [key, _] : base)
        baseKeys.insert(key);

    for (const auto& [key, _] : toCompare)
        toCompareKeys.insert(key);

    FileChecker::yamlComparatorElementsDelete(pm, commonKeys, baseKeys, toCompareKeys );

    yamlComparatorElementsAddAndStart(pm, toCompare, commonKeys, baseKeys, toCompareKeys);

}

void FileChecker::yamlComparatorElements( std::set<std::string>& commonKeys,
        std::map<std::string, ProgramConfig> base, std::map<std::string, ProgramConfig> toCompare ) {

    std::set<std::string> tempKeys;

    for (std::set<std::string>::iterator it = commonKeys.begin() ; it != commonKeys.end(); ++it) {

        std::vector<int> baseCodes = base[*it].getExitcodes();
        std::vector<int> toCompareCodes = toCompare[*it].getExitcodes();

        std::sort(baseCodes.begin(), baseCodes.end());
        std::sort(toCompareCodes.begin(), toCompareCodes.end());

        if (base[*it].getCmd() != toCompare[*it].getCmd()
                || base[*it].getNumprocs() != toCompare[*it].getNumprocs()
                || base[*it].getUmask() != toCompare[*it].getUmask()
                || base[*it].getWorkingdir() != toCompare[*it].getWorkingdir()
                || base[*it].getAutostart() != toCompare[*it].getAutostart()
                || baseCodes != toCompareCodes
                || base[*it].getStartretries() != toCompare[*it].getStartretries()
                || base[*it].getStarttime() != toCompare[*it].getStarttime()
                || base[*it].getAutorestart() != toCompare[*it].getAutorestart()
                || base[*it].getStopsignal() != toCompare[*it].getStopsignal()
                || base[*it].getStoptime() != toCompare[*it].getStoptime()
                || base[*it].getStdoutFile() != toCompare[*it].getStdoutFile()
                || base[*it].getStderrFile() != toCompare[*it].getStderrFile()
                || base[*it].getEnv() != toCompare[*it].getEnv()) {
            tempKeys.insert(*it);
            continue;
        }
        
    }

    commonKeys = tempKeys;
}

void    FileChecker::yamlComparatorElementsDelete( ProcessManager& pm,
            std::set<std::string> commonKeys, std::set<std::string> baseKeys,
            std::set<std::string> toCompareKeys ) {
    std::set<std::string> onlyInBase;
    std::vector<int> pid_list;

    std::set_difference(baseKeys.begin(), baseKeys.end(), toCompareKeys.begin(), toCompareKeys.end(),
                        std::inserter(onlyInBase, onlyInBase.begin()));

    onlyInBase.insert(commonKeys.begin(), commonKeys.end());

    for (const auto& k : onlyInBase) {
        for (const auto& pair : pm.getProcesses()) {
            if (pair.second.getName() == k) {
                pid_list.push_back(pair.first);

                if (kill(pair.first, pair.second.getConfig().getSig()[pair.second.getConfig().getStopsignal()]) == 0)
                    std::cout << "stopped: " << k << std::endl;

                if (kill(pair.first, SIGKILL) == 0)
                    std::cout << "killed " << k << std::endl;
            }
        }
    }

    for (const auto& k : pid_list)
        pm.getProcesses().erase(k);

    for (const auto& k : onlyInBase) {
        auto& list = Shell::get_programs_list();
        list.erase(std::remove(list.begin(), list.end(), k), list.end());

        pm.getConfig().getPrograms().erase(k);
    }
}

void    FileChecker::yamlComparatorElementsAddAndStart(  ProcessManager& pm,
            std::map<std::string, ProgramConfig> toCompare,
            std::set<std::string> commonKeys, std::set<std::string> baseKeys,
            std::set<std::string> toCompareKeys ) {
    std::set<std::string> onlyInToCompare;

    std::set_difference(toCompareKeys.begin(), toCompareKeys.end(), baseKeys.begin(), baseKeys.end(),
                        std::inserter(onlyInToCompare, onlyInToCompare.begin()));

    onlyInToCompare.insert(commonKeys.begin(), commonKeys.end());

    if (!onlyInToCompare.empty()) {
        for (const auto& k : onlyInToCompare) {
            auto& list = Shell::get_programs_list();
            list.push_back(k);

            pm.getConfig().getPrograms()[k] = toCompare[k];
        }

        std::map<std::string, ProgramConfig> programs;

        for (const auto& k : onlyInToCompare)
            programs[k] = pm.getConfig().getPrograms()[k];

        for(const auto& [name, progConf] : programs) {
            if (!progConf.getAutostart()) continue;

            for (int i = 0; i < progConf.getNumprocs(); i++) {
                Process process = pm.createProcess(progConf);
                if (process.getPid() == -1) continue;

                pm.getProcesses().emplace(process.getPid(), process);
            }
        }
    }
}

const char *FileChecker::NoFileException::what( void ) const throw() {
	return "Please enter the name of the configuration file (.yml or .yaml) as an argument.";
}

const char *FileChecker::NotRegularFileException::what( void ) const throw() {
	return "The path does not point to a regular file.";
}

const char *FileChecker::ReadableFileException::what( void ) const throw() {
	return "The file exists but cannot be read (see file permissions).";
}
