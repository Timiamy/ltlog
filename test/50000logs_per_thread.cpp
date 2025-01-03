#include <iostream>
#include <ltlog.hpp>
#include <vector>
#include <cstdint>
#include <iostream>
#include <chrono>

int main(int argc, char const *argv[])
{
    (void) argc;
    (void) argv;

    SetLtLogPath("./50000logs_per_thread.log");

    constexpr size_t kMaxLogPerThread = 50000;
    uint32_t cpu_cores = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(cpu_cores);

    auto start = std::chrono::system_clock::now();

    for(auto& t : threads)
    {
        t = std::thread([]()
        {
            for (size_t i = 0; i < kMaxLogPerThread; i++)
            {
                LogDebug("the " + std::to_string(i) + " log");
            }
        });
    }
    
    for(auto& t : threads)
    {
        if(t.joinable())
        {
            t.join();
        }
    }
    auto end1 = std::chrono::system_clock::now();


    std::cout << "Use " << cpu_cores << " threads generate " << cpu_cores * kMaxLogPerThread <<  " logs success!" << std::endl;
    std::cout << "Cost time:" << (end1-start).count()/1000/1000 << " ms" << std::endl;

    //Do NOT call destructor youself, I just want to calculate the time cost!
    //This will be a coredump after the main, because of the double free, but forget it because we just want know the cost time.
    Ltlog::LtLogger::GetInstance().~LtLogger();

    auto end2 = std::chrono::system_clock::now();

    std::cout << "Write " << cpu_cores * kMaxLogPerThread <<  " logs to file success!" << std::endl;
    std::cout << "Cost time:" << (end2-start).count()/1000/1000 << " ms" << std::endl;
    return 0;
}
