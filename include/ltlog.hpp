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

namespace Ltlog {

static constexpr auto kLogFilePath = "./ltlog.log";
static constexpr auto kMaxWatingNanoSecond = 500 * 1000;

class LtLogger {
   public:

    static LtLogger& GetInstance()
    {
        std::call_once(once_flag_, [] {
            instance_ = std::unique_ptr<LtLogger>(new LtLogger());
        });

        return *instance_;
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
        std::filesystem::path log_file_path(kLogFilePath);
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
                sleep_nano = sleep_nano > kMaxWatingNanoSecond ? sleep_nano
                                                               : sleep_nano * 2;
            }

            NanoSleep(sleep_nano);
        }
    }

    std::atomic<bool> is_exit_;
    
    std::ofstream ofstream_;
    std::thread write_file_thread_;
    CocurrentQueue<LogContent> queue_;
    static std::once_flag once_flag_;
    static std::unique_ptr<LtLogger> instance_; 
};

std::once_flag LtLogger::once_flag_;
std::unique_ptr<LtLogger> LtLogger::instance_;


#define log(msg)                                        \
{                                                       \
    Ltlog::LogContent log_content{};                    \
    log_content.filename = __FILE__;                    \
    log_content.line = __LINE__;                        \
    log_content.pid = Ltlog::GetPid();                  \
    log_content.time = Ltlog::GetTimePoint();           \
    log_content.message = msg;                          \
    Ltlog::LtLogger::GetInstance().add(log_content);    \
}                                                      
    

}  // namespace Ltlog