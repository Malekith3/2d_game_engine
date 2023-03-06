#ifndef LOGGER_H
#define LOGGER_H

#include<string>

class Logger{
    public:
        static void Log(const std::string& message);
        static void Err(const std::string& message);
    private:
        static std::string GetTimeStemp();
};
#endif // LOGGER_H

