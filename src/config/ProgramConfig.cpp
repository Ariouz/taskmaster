#include "ProgramConfig.hpp"


////////// Constructors | Destructor ////////// 

ProgramConfig::ProgramConfig( void ) = default;

ProgramConfig::ProgramConfig(const YAML::Node& node) {
    
    this->_cmd = node["cmd"].as<std::string>();
    this->_numprocs = node["numprocs"].as<int>();
    this->_umask = node["umask"].as<mode_t>();
    this->_workingdir = node["workingdir"].as<std::string>();
    this->_autostart = node["autostart"].as<bool>();
    this->_autorestart = parseAutoRestart(node["autorestart"].as<std::string>());
    if (node["exitcodes"]) {
        if (node["exitcodes"].IsSequence()) {
            this->_exitcodes = node["exitcodes"].as<std::vector<int>>();
        } else {
            this->_exitcodes = { node["exitcodes"].as<int>() };
        }
    }
    this->_startretries = node["startretries"].as<int>();
    this->_starttime = node["starttime"].as<int>();
    this->_stopsignal = node["stopsignal"].as<std::string>();
    this->_stoptime = node["stoptime"].as<int>();
    this->_stdout_file = node["stdout"].as<std::string>();
    this->_stderr_file = node["stderr"].as<std::string>();
    if (node["env"]) {
        this->_env = node["env"].as<std::map<std::string, std::string>>();
    }
}

ProgramConfig::~ProgramConfig( void ) = default;


////////// Getters ////////// 

std::string ProgramConfig::getProgramName( void ) const {
    return  this->_program_name;
}

std::string ProgramConfig::getCmd( void ) const {
    return this->_cmd;
}

int ProgramConfig::getNumprocs( void ) const {
    return this->_numprocs;
}

mode_t ProgramConfig::getUmask( void ) const {
    return this->_umask;
}

std::string ProgramConfig::getWorkingdir( void ) const {
    return this->_workingdir;
}

bool ProgramConfig::getAutostart( void ) const {
    return this->_autostart;
}

AutoRestart ProgramConfig::getAutorestart( void ) const {
    return this->_autorestart;
}

std::vector<int> ProgramConfig::getExitcodes( void ) const {
    return this->_exitcodes;
}

int ProgramConfig::getStartretries( void ) const {
    return this->_startretries;
}

int ProgramConfig::getStarttime( void ) const {
    return this->_starttime;
}

std::string ProgramConfig::getStopsignal( void ) const {
    return this->_stopsignal;
}

int ProgramConfig::getStoptime( void ) const {
    return this->_stoptime;
}

std::string ProgramConfig::getStdoutFile ( void ) const {
    return this->_stdout_file;
}

std::string ProgramConfig::getStderrFile( void ) const {
    return this->_stderr_file;
}

std::map<std::string, std::string> ProgramConfig::getEnv( void ) const {
    return this->_env;
}


////////// Setters ////////// 

void ProgramConfig::setProgramName(const std::string& program_name) {
    this->_program_name = program_name;
}

void ProgramConfig::setCmd(const std::string& cmd) {
    this->_cmd = cmd;
}

void ProgramConfig::setNumprocs(int numprocs) {
    this->_numprocs = numprocs;
}

void ProgramConfig::setUmask(mode_t umask) {
    this->_umask = umask;
}

void ProgramConfig::setWorkingdir(const std::string& workingdir) {
    this->_workingdir = workingdir;
}

void ProgramConfig::setAutostart(bool autostart) {
    this->_autostart = autostart;
}

void ProgramConfig::setAutorestart(AutoRestart& autorestart) {
    this->_autorestart = autorestart;
}

void ProgramConfig::setExitcodes(const std::vector<int>& exitcodes) {
    this->_exitcodes = exitcodes;
}

void ProgramConfig::setStartretries(int startretries) {
    this->_startretries = startretries;
}

void ProgramConfig::setStarttime(int starttime) {
    this->_starttime = starttime;
}

void ProgramConfig::setStopsignal(const std::string& stopsignal) {
    this->_stopsignal = stopsignal;
}

void ProgramConfig::setStoptime(int stoptime) {
    this->_stoptime = stoptime;
}

void ProgramConfig::setStdoutFile(const std::string& stdout_file) {
    this->_stdout_file = stdout_file;
}

void ProgramConfig::setStderrFile(const std::string& stderr_file) {
    this->_stderr_file = stderr_file;
}

void ProgramConfig::setEnv(const std::map<std::string, std::string>& env) {
    this->_env = env;
}


////////// Functions ////////// 

AutoRestart ProgramConfig::parseAutoRestart(const std::string& s) {
    if (s == "never") return AutoRestart::NEVER;
    if (s == "unexpected") return AutoRestart::UNEXPECTED;
    if (s == "always") return AutoRestart::ALWAYS;
    throw std::invalid_argument("Invalid autorestart value: " + s);
}
