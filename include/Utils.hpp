#pragma once
#include <chrono>
#include <thread>
#include <unistd.h>

namespace ltlog {
using TimePoint = std::chrono::_V2::system_clock::time_point;

#define unlikely(x) __builtin_expect(!!(x), 0)

inline void NanoSleep(int nano) {
    std::this_thread::sleep_for(std::chrono::nanoseconds(nano));
}

inline pid_t GetPid()
{
    return getpid();
}

inline TimePoint GetTimePoint()
{
    return std::chrono::system_clock::now();
}

}  // namespace ltlog
