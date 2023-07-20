#pragma once

#ifdef NDEBUG
#include <iostream>
#include <typeinfo>

template <class T, class U>
T checked_cast(U obj)
{
    T casted = dynamic_cast<T>(obj);
    if (!casted)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " checked_cast from " << typeid(U).name() << " to "
                  << typeid(T).name() << " failed !" << std::endl;
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
