#pragma once

#include <iostream>

#ifdef NDEBUG
#define HATCHER_ASSERT(condition)                                                                  \
    if (!(condition))                                                                              \
    {                                                                                              \
        std::cerr << __FILE__ << ":" << __LINE__ << " assertion (" << #condition << ") failed !"   \
                  << std::endl;                                                                    \
    }

#else // NDEBUG
#define HATCHER_ASSERT(condition)
#endif // !NDEBUG
