//
// Created by Alex Bordeaux on 25/06/2023.
//

#ifndef ANOMALY_DETECTION_MODULE_ADE_LOGGER_H
#define ANOMALY_DETECTION_MODULE_ADE_LOGGER_H
#include <memory>
#include <spdlog/spdlog.h>

class Logger {
public:
    Logger() = default;
    ~Logger() = default;

    static const std::shared_ptr<spdlog::logger> &getCoreLogger();
    static void Initialize();

private:
    static std::shared_ptr<spdlog::logger> core_logger;
};

#define LOGGER_TRACE(...) Logger::getCoreLogger()->trace(__VA_ARGS__)
#define LOGGER_DEBUG(...) Logger::getCoreLogger()->debug(__VA_ARGS__)
#define LOGGER_INFO(...)  Logger::getCoreLogger()->info(__VA_ARGS__)
#define LOGGER_WARN(...)  Logger::getCoreLogger()->warn(__VA_ARGS__)
#define LOGGER_ERROR(...) Logger::getCoreLogger()->error(__VA_ARGS__)
#define LOGGER_FATAL(...) Logger::getCoreLogger()->fatal(__VA_ARGS__)

#endif //ANOMALY_DETECTION_MODULE_ADE_LOGGER_H
