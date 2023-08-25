#include <Log.h>
#include <chrono>


Coconut::Log::Log() {}

Coconut::Log::~Log() {}

//unsave
void Coconut::Log::dump(std::string msg) {
    
    std::cout << msg << std::endl;
}


Coconut::Log::m_TimeInfo Coconut::Log::m_getCurrentTime() {
    using TimeInfo = Coconut::Log::m_TimeInfo;

    auto currentTime = std::chrono::system_clock::now();
    std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
    std::tm localTime = *std::localtime(&currentTimeT);

    TimeInfo timeInfo;
    timeInfo.year = localTime.tm_year + 1900;
    timeInfo.month = localTime.tm_mon + 1;
    timeInfo.day = localTime.tm_mday;
    timeInfo.hour = localTime.tm_hour;
    timeInfo.minute = localTime.tm_min;
    timeInfo.second = localTime.tm_sec;

    return timeInfo;

}