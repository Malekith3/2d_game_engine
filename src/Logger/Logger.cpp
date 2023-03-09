#include "../Logger/Logger.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>

std::vector<LogEntry> Logger::messages;

const std::string redColor     = "\x1b[31m";
const std::string greenColor   = "\x1b[32m";
const std::string defualtColor = "\x1b[0m";

std::string Logger::GetTimeStemp()
{
    // Get the current time point
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    // Get the current time point
    std::time_t time_t_now = std::chrono::system_clock::to_time_t(now);

    // Convert the time_t value to a tm struct
    std::tm *tm_now = std::localtime(&time_t_now);

    // Convert the time_t value to a tm struct
    long long ms_part = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;

    // Format the date and time string
    std::ostringstream oss;
    oss << std::put_time(tm_now, "%d/%m/%Y %H:%M:%S") << "." << std::setw(3) << std::setfill('0') << ms_part;

    return oss.str();
}

void Logger::Log(const std::string &message)
{
    const std::string timeStamp = Logger::GetTimeStemp();
    std::string new_message = "[LOG]   " + timeStamp + " " + message;
    LogEntry logEntry = {LOG_INFO, new_message};
    std::cout << greenColor << new_message << defualtColor << std::endl;
    messages.push_back(logEntry);      
}

void Logger::Err(const std::string &message)
{
    const std::string timeStamp = Logger::GetTimeStemp();
    std::string new_message = "[ERROR] " + timeStamp + " " + message;
    LogEntry logEntry = {LOG_ERROR, new_message};
    std::cerr << redColor << new_message << defualtColor << std::endl;
    messages.push_back(logEntry);
}

