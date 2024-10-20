#pragma once
#include <definitions.h>
#include <future>

class AsyncSignal
{
private:
    std::promise<bool> promiseObj;
    std::future<bool> futureObj;

public:
    AsyncSignal();
    void send();
    bool recieve(uint32_t timeInMs);
};
