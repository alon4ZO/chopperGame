#pragma once
#include <definitions.h>
#include <cassert>

class Manager
{
private:
    static bool isInitialized;

public:
    Manager();
    void Start();
};
