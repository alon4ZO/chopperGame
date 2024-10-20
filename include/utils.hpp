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

// Just in case CPP is older than 17 used explicit function.
float clamp(float value, float minVal, float maxVal);