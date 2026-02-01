#include "UnitTester.hpp"

#include <iomanip>
#include <iostream>
#include <limits>

namespace
{
class UnitTester
{
public:
    UnitTester() { std::cerr << std::setprecision(std::numeric_limits<float>::digits); }

    void RegisterFail() { m_errorCount++; }
    int GetErrorCount() const { return m_errorCount; }

private:
    int m_errorCount = 0;
};

UnitTester unitTester;
} // namespace

void RegisterFail()
{
    unitTester.RegisterFail();
}

int GetErrorCount()
{
    return unitTester.GetErrorCount();
}