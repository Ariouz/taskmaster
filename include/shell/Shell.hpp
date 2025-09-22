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
#include "ProcessManager.hpp"


class Shell {

    private:
        char*                                               _readline_return;
        static std::vector<std::string>                     _commands;
        static std::map<std::string, std::function<void(const std::string& arg)>> _commands_functions;

        ProcessManager*                                     _pm;

    public:
        Shell() = default;
        Shell(ProcessManager* pm);
        ~Shell();

        static char*    command_generator( const char* text, int state );
        static char**   custom_completion( const char* text, int start, int end );
        void            functionsCall( const std::string readline_return ) const;
        void            run( const std::string& arg );
        void            status( void );
        void            start( const std::string& arg );
        void            stop( const std::string& arg );
        void            reload( void );
        void            quit( void );

};
