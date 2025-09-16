#include <string>

class Logger {

    private:
        std::string _logfile;

    public:
        Logger();
        Logger(const std::string& logfile);
        ~Logger();

        void info(std::string message);
        void warn(std::string message);
        void error(std::string message);

};