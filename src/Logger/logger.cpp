//
// Created by Alex Bordeaux on 25/06/2023.
//


#include "logger.h"
#include <stdexcept>
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Logger::core_logger = nullptr;

const std::shared_ptr<spdlog::logger> &Logger::getCoreLogger() {
  if(core_logger == nullptr){
    throw std::runtime_error("Logger was referenced before initialising");
  }
    return core_logger;
}

void Logger::Initialize() {
    // Format need to add before
    spdlog::set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v%$");
    core_logger = spdlog::stdout_color_mt("Anomaly_Detection_Engine_Logger");
    spdlog::set_level(spdlog::level::trace);
    LOGGER_TRACE("Logger was initialised");
}
