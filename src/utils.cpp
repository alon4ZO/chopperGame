#include <utils.hpp>
#include <algorithm>
#include <filesystem>

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
    auto timeoutDuration = std::chrono::milliseconds(850); // ALONB - what is this time?
    return (futureObj.wait_for(timeoutDuration) == std::future_status::ready);
}

float clamp(float value, float minVal, float maxVal) // Just in case CPP is older than 17 used explicit function.
{
    return std::max(minVal, std::min(maxVal, value));
}

string getPathForAsset(string fileName, string postfix, uint8_t optionalArg)
{

    const char *currentFilePath = __FILE__;
    uint8_t option;
    string flavor = "";

    // Using filesystem to manipulate paths (C++17 and later)
    std::filesystem::path path(currentFilePath);
    std::filesystem::path parentDirectory = path.parent_path().parent_path();

    if (optionalArg != 1)
    {
        option = getRandomNumber((0), optionalArg - 1);
        flavor = "_" + to_string(option);
    }

    // Construct a relative path from the current file location
    std::filesystem::path relativePath = parentDirectory / "assets" / (fileName + flavor + postfix);

    // Outputting the resolved relative path
    string res = relativePath.u8string();

    for (char &c : res)
    {
        if (c == '\\')
        {
            c = '/';
        }
    }

    return res;
}