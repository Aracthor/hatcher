#include "UnitTester.hpp"

#include <iomanip>
#include <iostream>
#include <limits>

namespace
{
int s_errorCount = 0;
} // namespace

RegisteredTest* RegisteredTest::s_first = nullptr;
RegisteredTest::RegisteredTest(std::function<void()> test)
    : test(test)
    , next(s_first)
{
    s_first = this;
}

void RegisterFail()
{
    s_errorCount++;
}

int main()
{
    std::cerr << std::setprecision(std::numeric_limits<float>::digits);
    const RegisteredTest* test = RegisteredTest::s_first;
    while (test)
    {
        test->test();
        test = test->next;
    }
    return s_errorCount;
}
