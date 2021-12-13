#pragma once

#include <cstring>
#include <iostream>

#define PTHREAD_CHECK(function)                                                                    \
    int functionRet = function;                                                                    \
    if (functionRet != 0)                                                                          \
    {                                                                                              \
        std::cerr << __FILE__ << ":" << __LINE__ << " " << #function                               \
                  << " ERROR: " << strerror(functionRet) << std::endl;                             \
        std::terminate();                                                                          \
    }
