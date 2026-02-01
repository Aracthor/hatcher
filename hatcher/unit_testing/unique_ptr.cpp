#include "UnitTester.hpp"

#include "hatcher/unique_ptr.hpp"

using namespace hatcher;

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

void testUniquePtr()
{
    {
        unique_ptr<Counter> ptr;
        ptr = make_unique<Counter>();
        TEST_EQUALS(Counter::count, 1);
    }
    TEST_EQUALS(Counter::count, 0);

    {
        unique_ptr<Counter> ptr1 = make_unique<Counter>();
        unique_ptr<Counter> ptr2 = MakeCounter();
        TEST_EQUALS(Counter::count, 2);
    }
    TEST_EQUALS(Counter::count, 0);

    {
        unique_ptr<Interface> ptr1 = make_unique<Counter>();
        TEST_EQUALS(Counter::count, 1);
    }
    TEST_EQUALS(Counter::count, 0);
}
