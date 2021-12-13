#pragma once

#include <pthread.h> // TODO move this include to a .cpp only.

#include <functional>

namespace hatcher
{

class Thread
{
public:
    using Routine = void* (*)(void*);

    Thread(Routine routine, void* argument = nullptr);
    ~Thread();

private:
    static pthread_attr_t s_attr;
    static int s_threadCount;

    pthread_t m_thread;
};

} // namespace hatcher
