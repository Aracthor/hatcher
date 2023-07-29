#include <iomanip>
#include <iostream>
#include <limits>

#include "hatcher/unique_ptr.hpp"

using namespace hatcher;

namespace
{
template <typename T>
int testEquals(const T& a, const T& b)
{
    if (a != b)
        std::cerr << std::setprecision(std::numeric_limits<float>::digits) << "unit test fail: " << a << " != " << b
                  << std::endl;
    return a != b;
}

struct Interface
{
    virtual ~Interface() = default;
};

struct Counter : public Interface
{
    static int count;

    Counter() { count++; }
    ~Counter() { count--; }
};
int Counter::count = 0;

unique_ptr<Counter> MakeCounter()
{
    return make_unique<Counter>();
}
} // namespace

int testUniquePtr()
{
    int fails = 0;
    {
        unique_ptr<Counter> ptr;
        ptr = make_unique<Counter>();
        fails += testEquals(Counter::count, 1);
    }
    fails += testEquals(Counter::count, 0);

    {
        unique_ptr<Counter> ptr1 = make_unique<Counter>();
        unique_ptr<Counter> ptr2 = MakeCounter();
        fails += testEquals(Counter::count, 2);
    }
    fails += testEquals(Counter::count, 0);

    {
        unique_ptr<Interface> ptr1 = make_unique<Counter>();
        fails += testEquals(Counter::count, 1);
    }
    fails += testEquals(Counter::count, 0);

    return fails;
}
