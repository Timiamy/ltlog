#include <iostream>
#include <ltlog.hpp>
#include <vector>
#include <cstdint>

int main(int argc, char const *argv[])
{
    constexpr size_t kMaxLogPerThread = 5000;
    uint32_t cpu_cores = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(cpu_cores);

    for(auto& t : threads)
    {
        t = std::thread([]()
        {
            for (size_t i = 0; i < kMaxLogPerThread; i++)
            {
                log("the " + std::to_string(i) + " log");
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

    std::cout << "All logs generate success!" << std::endl;
    
    return 0;
}
