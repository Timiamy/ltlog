#pragma once
#include <CocurrentQueue.hpp>
#include <LogContent.hpp>
#include <Utils.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <thread>
#include <mutex>
#include <memory>
#include <config.hpp>

namespace Ltlog {

class LtLogger {
   public:

    static LtLogger& GetInstance()
    {
        std::call_once(once_flag_, [] {
            instance_ = std::unique_ptr<LtLogger>(new LtLogger());
        });

        return *instance_;
    }

    static void SetLogPath(std::string log_path)
    {
        log_path_ = std::move(log_path);
    }

    void add(LogContent&& log_content)
    {
        queue_.Push(std::move(log_content));
    }

    void add(const LogContent& log_content)
    {
        queue_.Push(log_content);
    }

    ~LtLogger() {
        is_exit_.store(true, std::memory_order_relaxed);
        if (write_file_thread_.joinable()) {
            write_file_thread_.join();
        }
    }
   private:
    LtLogger()
        : is_exit_(false),
          write_file_thread_([this]() { this->WriteLogProc(); }) {
        std::string log_path = !log_path_.empty() ? log_path_: kLogFilePath;
        std::filesystem::path log_file_path(log_path_);
        if (!std::filesystem::exists(log_file_path)) {
            if (!std::filesystem::exists(log_file_path.parent_path())) {
                if (!std::filesystem::create_directories(
                        log_file_path.parent_path())) {
                    std::cout << "Can not create file path" << std::endl;
                    log_file_path = std::filesystem::path("./ltlog.log");
                }
            }
        }

        ofstream_.open(log_file_path, std::ios::app | std::ios::out);
    }

    void WriteLogProc() {
        int sleep_nano = 1;
        while (true) {
            auto log_content = queue_.Pop();
            if (log_content.has_value()) {
                ofstream_ << log_content.value().format();
                sleep_nano = sleep_nano <= 1 ? sleep_nano : sleep_nano / 2;
            } else {
                if (unlikely(is_exit_.load(std::memory_order_relaxed))) {
                    ofstream_.flush();
                    ofstream_.close();
                    return;
                }
                sleep_nano = sleep_nano > kMaxWaitingNanoSecond ? sleep_nano
                                                               : sleep_nano * 2;
            }

            NanoSleep(sleep_nano);
        }
    }

    std::atomic<bool> is_exit_;
    
    std::ofstream ofstream_;
    std::thread write_file_thread_;
    CocurrentQueue<LogContent> queue_;
    static std::string log_path_;
    static std::once_flag once_flag_;
    static std::unique_ptr<LtLogger> instance_; 
};

std::string LtLogger::log_path_;
std::once_flag LtLogger::once_flag_;
std::unique_ptr<LtLogger> LtLogger::instance_;

#define SetLtLogPath(x)                                 \
    Ltlog::LtLogger::SetLogPath(x)

#define Log(msg, lv)                                    \
{                                                       \
    Ltlog::LogContent log_content{};                    \
    log_content.level = lv;                             \
    log_content.filename = __FILE__;                    \
    log_content.line = __LINE__;                        \
    log_content.pid = Ltlog::GetPid();                  \
    log_content.time = Ltlog::GetTimePoint();           \
    log_content.message = msg;                          \
    Ltlog::LtLogger::GetInstance()                      \
        .add(std::move(log_content));                   \
}                                                      
    
#define LogDebug(msg)                                   \
    Log(msg, Ltlog::LogLevel::kLogDebug)

#define LogInfo(msg)                                    \
    Log(msg, Ltlog::LogLevel::kLogInfo)

#define LogWarning(msg)                                 \
    Log(msg, Ltlog::LogLevel::kLogWarning)

#define LogError(msg)                                   \
    Log(msg, Ltlog::LogLevel::kLogError)

#define LogCritical(msg)                                \
    Log(msg, Ltlog::LogLevel::kLogCritical)
}  // namespace Ltlog