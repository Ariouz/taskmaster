#pragma once

#include <iostream>
#include <string>
#include <readline/readline.h>
#include <readline/history.h>
#include <vector>
#include <map>
#include <functional>
#include <sstream>
#include <mutex>
#include <atomic>
#include <thread>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include "ProcessManager.hpp"
#include "Logger.hpp"
#include "Status.hpp"
#include "FileChecker.hpp"
#include "SigUtils.hpp"

extern std::atomic<bool> logs_mode_flag;

class Shell {

    private:
        char*                                                                       _readline_return;
        static std::vector<std::string>                                             _commands;
        static std::vector<std::string>                                             _programs_list;
        static const std::vector<std::string>*                                      _current_completion_list;
        static std::map<std::string, std::function<void(const std::string& arg)>>   _commands_functions;
        ProcessManager*                                                             _pm;
        std::string                                                                 _configFile;
        Config                                                                      _temp_config_reload;

        std::vector<std::string>    _initArgs( const std::string& arg );

    public:
        Shell() = default;
        Shell(const std::string& configFile, ProcessManager* pm);
        ~Shell();

        static std::vector<std::string>&    get_programs_list( void );

        void                        set_termios_handle( void );
        static void                 sigint_handler( int signum );
        void                        signal( void );
        static char*                command_generator( const char* text, int state );
        static char**               custom_completion( const char* text, int start, int end );
        std::vector<std::string>    split_multi_delim(const std::string& str, const std::string& delimiters);
        void                        functionsCall( const std::string readline_return ) const;
        void                        init_commands_functions_map( void );
        void                        init_programs_list_vector( void );
        void                        readline_util( void );

        void                        run( const std::string& arg );
        void                        status( const std::string& arg );
        void                        start( const std::string& arg );
        void                        stop( const std::string& arg );
        void                        reload( void );
        void                        quit( void );

        #ifdef BONUS
        void            logs( void );
        #endif

};
