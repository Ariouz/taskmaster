#include "ProgramConfig.hpp"


////////// Constructors | Destructor ////////// 

ProgramConfig::ProgramConfig( void ) = default;

ProgramConfig::ProgramConfig(const std::string& name, const YAML::Node& node) {
    this->_program_name = name;
    this->initCmd( node );
    this->initNumprocs( node );
    this->initUmask( node );
    this->initWorkingdir( node );
    this->initAutostart( node );
    this->initAutorestart( node );
    this->initExitcodes( node );
    this->initStartretries( node );
    this->initStarttime( node );
    this->initStopsignal( node );
    this->initStoptime( node );
    this->initStdoutFile( node );
    this->initStderrFile( node );
    this->initEnv( node );
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

void    ProgramConfig::initCmd( const YAML::Node& node ) {
    try
    {
        this->_cmd = node["cmd"].as<std::string>();
        if (this->_cmd == "null") {
            std::cerr << "[WARNING] Please initiate cmd field of " + this->_program_name + " program on configuration file." << std::endl;
            exit (EXIT_FAILURE);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "[WARNING] Please initiate cmd field of " + this->_program_name + " program on configuration file." << std::endl;
        exit (EXIT_FAILURE);
    }
}

void    ProgramConfig::initNumprocs( const YAML::Node& node ) {
    try
    {
        this->_numprocs = node["numprocs"].as<int>();
    }
    catch (std::exception& e)
    {
        std::string msg = "Please initiate numprocs field of " + this->_program_name + "program on configuration file.";
        Logger::warn(msg);
    }
}

void    ProgramConfig::initUmask( const YAML::Node& node ) {
    try
    {
        this->_umask = node["umask"].as<mode_t>();
    }
    catch (std::exception& e)
    {
        std::string msg = "Please initiate umask field of " + this->_program_name + "program on configuration file.";
        Logger::warn(msg);
    }
}

void    ProgramConfig::initWorkingdir( const YAML::Node& node ) {
    try
    {
        this->_workingdir = node["workingdir"].as<std::string>();
        if (this->_workingdir == "null")
            this->_workingdir = ".";
    }
    catch (std::exception& e)
    {
        std::string msg = "Please initiate workingdir field of " + this->_program_name + "program on configuration file.";
        Logger::warn(msg);
    }
}

void    ProgramConfig::initAutostart( const YAML::Node& node ) {
    try
    {
        this->_autostart = node["autostart"].as<bool>();
    }
    catch (std::exception& e)
    {
        std::string msg = "Please initiate autostart field of " + this->_program_name + "program on configuration file.";
        Logger::warn(msg);
    }
}

void    ProgramConfig::initAutorestart( const YAML::Node& node ) {
    try
    {
        this->_autorestart = parseAutoRestart(node["autorestart"].as<std::string>());
    }
    catch (std::exception& e)
    {
        std::string msg = "Please initiate autorestart field of " + this->_program_name + "program on configuration file.";
        Logger::warn(msg);
    }
}

AutoRestart ProgramConfig::parseAutoRestart(const std::string& s) {
    if (s == "never") return AutoRestart::NEVER;
    if (s == "unexpected") return AutoRestart::UNEXPECTED;
    if (s == "always") return AutoRestart::ALWAYS;
    throw std::invalid_argument("Invalid autorestart value: " + s);
}

void    ProgramConfig::initExitcodes( const YAML::Node& node ) {
    try
    {
        if (node["exitcodes"]) {
            if (node["exitcodes"].IsSequence()) {
                this->_exitcodes = node["exitcodes"].as<std::vector<int>>();
            } else if (node["exitcodes"].IsScalar()) {
                this->_exitcodes = { node["exitcodes"].as<int>() };
            } else {
                this->_exitcodes = { node["exitcodes"].as<int>() };
            }
        }
    }
    catch (std::exception& e)
    {
        std::string msg = "Please initiate exitcodes field of " + this->_program_name + "program on configuration file.";
        Logger::warn(msg);
    }
}

void    ProgramConfig::initStartretries( const YAML::Node& node ) {
    try
    {
        this->_startretries = node["startretries"].as<int>();
    }
    catch (std::exception& e)
    {
        std::string msg = "Please initiate startretries field of " + this->_program_name + "program on configuration file.";
        Logger::warn(msg);
    }
}

void    ProgramConfig::initStarttime( const YAML::Node& node ) {
    try
    {
        this->_starttime = node["starttime"].as<int>();
    }
    catch (std::exception& e)
    {
        std::string msg = "Please initiate starttime field of " + this->_program_name + "program on configuration file.";
        Logger::warn(msg);
    }
}

void    ProgramConfig::initStopsignal( const YAML::Node& node ) {
    try
    {
        this->_stopsignal = node["stopsignal"].as<std::string>();

        if (this->_stopsignal != "HUP" && this->_stopsignal != "INT"
                && this->_stopsignal != "QUIT" && this->_stopsignal != "USR1"
                && this->_stopsignal != "USR2" && this->_stopsignal != "TERM") {
            this->_stopsignal = "TERM";
        }
    }
    catch (std::exception& e)
    {
        std::string msg = "Please initiate stopsignal field of " + this->_program_name + "program on configuration file.";
        Logger::warn(msg);
    }
}

void    ProgramConfig::initStoptime( const YAML::Node& node ) {
    try
    {
        this->_stoptime = node["stoptime"].as<int>();
    }
    catch (std::exception& e)
    {
        std::string msg = "Please initiate stoptime field of " + this->_program_name + "program on configuration file.";
        Logger::warn(msg);
    }
}

void    ProgramConfig::initStdoutFile( const YAML::Node& node ) {
    try
    {
        this->_stdout_file = node["stdout"].as<std::string>();
        if (this->_stdout_file == "null")
            this->_stdout_file = "/dev/null";
    }
    catch (std::exception& e)
    {
        std::string msg = "Please initiate stdout field of " + this->_program_name + "program on configuration file.";
        Logger::warn(msg);
    }
}

void    ProgramConfig::initStderrFile( const YAML::Node& node ) {
    try
    {
        this->_stderr_file = node["stderr"].as<std::string>();
        if (this->_stderr_file == "null")
            this->_stderr_file = "/dev/null";
    }
    catch (std::exception& e)
    {
        std::string msg = "Please initiate stderr field of " + this->_program_name + "program on configuration file.";
        Logger::warn(msg);
    }
}

void    ProgramConfig::initEnv( const YAML::Node& node ) {
    try
    {
        if (node["env"])
            this->_env = node["env"].as<std::map<std::string, std::string>>();
    }
    catch (std::exception& e)
    {
        std::string msg = "Please initiate env field of " + this->_program_name + "program on configuration file.";
        Logger::warn(msg);
    }
}
