#include "Mutex.hpp"

#include "pthread_check.h"

namespace hatcher
{

Mutex::Mutex()
{
    PTHREAD_CHECK(pthread_mutex_init(&m_mutex, nullptr));
}

Mutex::~Mutex()
{
    PTHREAD_CHECK(pthread_mutex_destroy(&m_mutex));
}

void Mutex::Lock()
{
    PTHREAD_CHECK(pthread_mutex_lock(&m_mutex));
}

void Mutex::Unlock()
{
    PTHREAD_CHECK(pthread_mutex_unlock(&m_mutex));
}

} // namespace hatcher
