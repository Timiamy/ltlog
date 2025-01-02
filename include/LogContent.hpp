#pragma once
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>

namespace Ltlog {
using TimePoint = std::chrono::_V2::system_clock::time_point;

struct LogContent {
    pid_t pid;
    int line;
    TimePoint time;
    const char* filename;
    std::string message;

    std::string format() {
        std::stringstream ss;
        auto time_t = std::chrono::system_clock::to_time_t(time);
        auto time_millis =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                time.time_since_epoch())
                .count() %
            1000;

        std::tm tm = *std::localtime(&time_t);

        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "."
                  << std::setw(3) << std::setfill('0') << time_millis;

        ss << " [" << pid << "] ";
        ss << filename << ":" << line << " ";
        ss << message << std::endl;

        return ss.str();
    }
};

};  // namespace Ltlog