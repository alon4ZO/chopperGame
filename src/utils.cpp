#include <utils.hpp>

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