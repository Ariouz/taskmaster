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

std::map<std::string, ProgramConfig> FileChecker::yamlComparator( ProcessManager& pm,
                                                                    std::map<std::string, ProgramConfig> toCompare ) {

    std::map<std::string, ProgramConfig> base;
    std::set<std::string> commonKeys, baseKeys, toCompareKeys, onlyInBase, onlyInToCompare;

    base = pm.getConfig().getPrograms();

    for (const auto& [key, _] : base)
        if (toCompare.find(key) != toCompare.end())
            commonKeys.insert(key);

    if (!commonKeys.empty())
        yamlComparatorElements(commonKeys, base, toCompare);

    for (const auto& [key, _] : base)
        baseKeys.insert(key);

    for (const auto& [key, _] : toCompare)
        toCompareKeys.insert(key);

    std::set_difference(baseKeys.begin(), baseKeys.end(), toCompareKeys.begin(), toCompareKeys.end(),
                        std::inserter(onlyInBase, onlyInBase.begin()));

    if (!onlyInBase.empty()) {
        for (const auto& k : onlyInBase) {
            for (auto it = pm.getProcesses().begin(); it != pm.getProcesses().end(); ) {
                if (it->second.getName() == k) {
                    if (kill(it->first, it->second.getConfig().getSig()[it->second.getConfig().getStopsignal()]) == 0)
                        std::cout << "stopped: " << k << std::endl;
                    else {
                        break;
                    }

                    if (kill(it->first, SIGKILL) == 0) {
                        std::cout << "killed " << k << std::endl;
                        auto it_tmp = std::next(it);
                        pm.getProcesses().erase(it->first);
                        it = it_tmp;
                    }
                    else {
                        break;
                    }
                }
                else
                    it++;
            }
            auto& list = Shell::get_programs_list();
            list.erase(std::remove(list.begin(), list.end(), k), list.end());

            pm.getConfig().getPrograms().erase(k);
        }
    }

    std::set_difference(toCompareKeys.begin(), toCompareKeys.end(), baseKeys.begin(), baseKeys.end(),
                        std::inserter(onlyInToCompare, onlyInToCompare.begin()));

    if (!onlyInToCompare.empty()) {
        /*
            Lancer les applications
        */
        std::cout << "Clés uniquement dans map2 :\n";
        for (const auto& k : onlyInToCompare) std::cout << "- " << k << "\n";
        //ajouter les services dans process et penser a les lancer avec les services qui ont ete modifies
        for (const auto& k : onlyInToCompare) {
            auto& list = Shell::get_programs_list();
            list.push_back(k);

            pm.getConfig().getPrograms()[k] = toCompare[k];
        }
    }

    //Traiter commonKeys afin pouvoir changer les atibuts de pm.getConfig().getPrograms() et egalement au niveau des
    //processes, puis lancer les services avec ceux qui n existe que dans la liste update.

    return base;
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

const char *FileChecker::NoFileException::what( void ) const throw() {
	return "Please enter the name of the configuration file (.yml or .yaml) as an argument.";
}

const char *FileChecker::NotRegularFileException::what( void ) const throw() {
	return "The path does not point to a regular file.";
}

const char *FileChecker::ReadableFileException::what( void ) const throw() {
	return "The file exists but cannot be read (see file permissions).";
}
