#include <ltlog.hpp>

int main(int argc, char const *argv[])
{
    (void) argc;
    (void) argv;
    //Default is ./ltlog.log if not set
    SetLtLogPath("./helloworld.log");
    LogDebug("Hello world!");
    LogInfo("Hello world!");
    LogWarning("Hello world!");
    LogError("Hello world!");
    LogCritical("Hello world!");
    return 0;
}
