#pragma once

#include "assert.hpp"

namespace hatcher
{

template <class T>
class unique_ptr
{
public:
    unique_ptr()
        : m_ptr(nullptr)
    {
    }

    template <class U>
    unique_ptr(U* ptr)
        : m_ptr(ptr)
    {
    }

    template <class U>
    unique_ptr(unique_ptr<U>&& other)
        : m_ptr(other.release())
    {
    }

    ~unique_ptr() { delete m_ptr; }

    unique_ptr<T>& operator=(unique_ptr<T>&& other)
    {
        m_ptr = other.release();
        return *this;
    }

    T& operator*() const
    {
        HATCHER_ASSERT(m_ptr != nullptr);
        return *m_ptr;
    }

    T* operator->() const
    {
        HATCHER_ASSERT(m_ptr != nullptr);
        return m_ptr;
    }

    operator bool() const { return m_ptr != nullptr; }

    T* get() const { return m_ptr; }

    T* release()
    {
        T* ptr = m_ptr;
        m_ptr = nullptr;
        return ptr;
    }

private:
    T* m_ptr;
};

template <class T, typename... Args>
unique_ptr<T> make_unique(Args&&... args)
{
    return unique_ptr<T>(new T(args...));
}

} // namespace hatcher
