#include "Logger.hpp"
#include <unistd.h>
#include <thread>
#include <vector>

using namespace std;
using namespace sinicheveen;

void writeLog()
{
    while (true)
    {
        GLOBAL_LOG_TRACE("level: %s, code: %d", "TRACE", 0);
        usleep(400);
    }
}

int main()
{
    Logger::globalRotatingLoggerInit("my_log", 5 * 1024 *1024, 5, LOG_LEVEL_E::TRACE);

#if 1    //���߳��÷�ʾ��
    vector<thread> vT;
    for (int i = 0; i < 4; i++)
    {
        vT.push_back(thread(writeLog));
    }

    for (auto& iT : vT)
    {
        iT.join();
    }
#endif

#if 0    //���߳��÷�ʾ��
    writeLog();
#endif
    
    return 0;
}