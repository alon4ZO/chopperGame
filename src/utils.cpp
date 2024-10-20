#include <utils.hpp>
#include <algorithm>

AsyncSignal::AsyncSignal()
{
    futureObj = promiseObj.get_future();
}

void AsyncSignal::send()
{
    promiseObj.set_value(true);
}

bool AsyncSignal::recieve(uint32_t timeInMs)
{
    auto timeoutDuration = std::chrono::milliseconds(850);
    return (futureObj.wait_for(timeoutDuration) == std::future_status::ready);
}

float clamp(float value, float minVal, float maxVal) // Just in case CPP is older than 17 used explicit function.
{
    return std::max(minVal, std::min(maxVal, value));
}