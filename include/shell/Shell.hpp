#pragma once

#include <iostream>
#include <string>
#include <readline/readline.h>
#include <readline/history.h>
#include <vector>
#include <map>
#include <functional>
#include <sstream>


class Shell {

    private:
        char*                                               _readline_return;
        static std::vector<std::string>                     _commands;
        static std::map<std::string, std::function<void()>> _commands_functions;

    public:
        Shell();
        ~Shell();

        static char*    command_generator( const char* text, int state );
        static char**   custom_completion( const char* text, int start, int end );
        void            functionsCall( const std::string readline_return ) const;
        void            run( void );
        void            status( void );
        void            start( void );
        void            stop( void );
        void            reload( void );
        void            quit( void );

};
