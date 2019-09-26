//
// Created by xinnjie on 2019/9/24.
//
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


void stdout_example()
{
    // create color multi threaded logger
    auto console = spdlog::stdout_color_mt("console");
    auto err_logger = spdlog::stderr_color_mt("stderr");
    spdlog::get("console")->info("loggers can be retrieved from a global registry using the spdlog::get(logger_name)");

}

int main() {
    SPDLOG_ERROR("Some debug message");

    return 0;
}