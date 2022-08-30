#pragma once

#ifdef NDEBUG
#include <iostream>

#define HATCHER_ASSERT(condition)                                                                  \
    if (!(condition))                                                                              \
    {                                                                                              \
        std::cerr << __FILE__ << ":" << __LINE__ << " assertion (" << #condition << ") failed !"   \
                  << std::endl;                                                                    \
    }

#define HATCHER_ASSERT_MESSAGE(condition, message)                                                 \
    if (!(condition))                                                                              \
    {                                                                                              \
        std::cerr << __FILE__ << ":" << __LINE__ << " assertion (" << #condition << ") failed !"   \
                  << std::endl                                                                     \
                  << message << std::endl;                                                         \
    }

#else // NDEBUG
#define HATCHER_ASSERT(condition)
#define HATCHER_ASSERT_MESSAGE(condition, message)
#endif // !NDEBUG
