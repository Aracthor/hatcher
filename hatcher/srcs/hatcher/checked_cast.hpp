#pragma once

#ifdef NDEBUG
#include <sstream>
#include <stdexcept>
#include <typeinfo>

template <class T, class U>
T checked_cast(U obj)
{
    T casted = dynamic_cast<T>(obj);
    if (!casted)
    {
        std::ostringstream oss;
        oss << __FILE__ << ":" << __LINE__ << " checked_cast from " << typeid(U).name() << " to " << typeid(T).name()
            << " failed !";
        throw std::runtime_error(oss.str());
    }
    return casted;
}

#else
template <class T, class U>
T checked_cast(U obj)
{
    return reinterpret_cast<T>(obj);
}

#endif
