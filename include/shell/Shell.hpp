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
#include "ProcessManager.hpp"
#include "Logger.hpp"
#include "Status.hpp"

extern std::atomic<bool> logs_mode_flag;

class Shell {

    private:
        char*                                                                       _readline_return;
        static std::vector<std::string>                                             _commands;
        static std::vector<std::string>                                             _programs_list;
        static const std::vector<std::string>*                                      _current_completion_list;
        static std::map<std::string, std::function<void(const std::string& arg)>>   _commands_functions;
        ProcessManager*                                                             _pm;

        std::vector<std::string>    _initArgs( const std::string& arg );

    public:
        Shell() = default;
        Shell(ProcessManager* pm);
        ~Shell();

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
