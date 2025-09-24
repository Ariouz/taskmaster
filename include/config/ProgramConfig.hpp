#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <yaml-cpp/yaml.h>
#include "Logger.hpp"

enum AutoRestart {
    NEVER,
    UNEXPECTED,
    ALWAYS
};

class ProgramConfig {
    private:
        std::string _program_name;
        std::string _cmd;
        int _numprocs = 1;
        mode_t _umask = 022;
        std::string _workingdir = ".";
        bool _autostart = false;
        AutoRestart _autorestart = AutoRestart::NEVER;
        std::vector<int> _exitcodes = {1};
        int _startretries = 3;
        int _starttime = 1;
        std::string _stopsignal = "TERM";
        int _stoptime = 10;
        std::string _stdout_file = "/dev/null";
        std::string _stderr_file = "/dev/null";
        std::map<std::string, std::string> _env;

        void    initNumprocs( const YAML::Node& node );
        void    initCmd( const YAML::Node& node );
        void    initUmask( const YAML::Node& node );
        void    initWorkingdir( const YAML::Node& node );
    
    public:
        ProgramConfig( void );
        ProgramConfig( const std::string& name, const YAML::Node& node );
        ~ProgramConfig( void );
        
        std::string                         getProgramName( void ) const;
        std::string                         getCmd( void ) const;
        int                                 getNumprocs( void ) const;
        mode_t                              getUmask( void ) const;
        std::string                         getWorkingdir( void ) const;
        bool                                getAutostart( void ) const;
        AutoRestart                         getAutorestart( void ) const;
        std::vector<int>                    getExitcodes( void ) const;
        int                                 getStartretries( void ) const;
        int                                 getStarttime( void ) const;
        std::string                         getStopsignal( void ) const;
        int                                 getStoptime( void ) const;
        std::string                         getStdoutFile ( void ) const;
        std::string                         getStderrFile( void ) const;
        std::map<std::string, std::string>  getEnv( void ) const;
        
        void setProgramName(const std::string& program_name);
        void setCmd(const std::string& cmd);
        void setNumprocs(int numprocs);
        void setUmask(mode_t umask);
        void setWorkingdir(const std::string& workingdir);
        void setAutostart(bool autostart);
        void setAutorestart(AutoRestart& autorestart);
        void setExitcodes(const std::vector<int>& exitcodes);
        void setStartretries(int startretries);
        void setStarttime(int starttime);
        void setStopsignal(const std::string& stopsignal);
        void setStoptime(int stoptime);
        void setStdoutFile(const std::string& stdout_file);
        void setStderrFile(const std::string& stderr_file);
        void setEnv(const std::map<std::string, std::string>& env);
        
        
        static AutoRestart parseAutoRestart(const std::string& s);
};
