#include "Thread.hpp"

#include "pthread_check.h"

#include "hatcher/assert.hpp"

namespace hatcher
{

pthread_attr_t Thread::s_attr;

int Thread::s_threadCount = 0;

Thread::Thread(Routine routine, void* argument /* = nullptr */)
{
    if (s_threadCount == 0)
    {
        PTHREAD_CHECK(pthread_attr_init(&s_attr));
    }

    PTHREAD_CHECK(pthread_create(&m_thread, &s_attr, routine, argument));
    s_threadCount++;
}

Thread::~Thread()
{
    void* dummyReturnValue;
    PTHREAD_CHECK(pthread_join(m_thread, &dummyReturnValue));
    HATCHER_ASSERT(dummyReturnValue == nullptr);

    s_threadCount--;
    if (s_threadCount == 0)
    {
        PTHREAD_CHECK(pthread_attr_destroy(&s_attr));
    }
}

} // namespace hatcher
