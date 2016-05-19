#pragma once

#include <chrono>
#include "libcoursework_api.h"

class LIBCOURSEWORK_API Benchmark{
public:
    static void Start();
    static double Stop();
};