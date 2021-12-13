#pragma once

#include <pthread.h> // TODO move this include to a .cpp only.

namespace hatcher
{

class Mutex
{
public:
    Mutex();
    ~Mutex();

    void Lock();
    void Unlock();

private:
    pthread_mutex_t m_mutex;
};

} // namespace hatcher
