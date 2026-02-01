#pragma once

#include <iostream>

void RegisterFail();
int GetErrorCount();

template <typename T>
void TestEquals(const T& a, const T& b, const char* file, int line)
{
    if (a != b)
    {
        std::cerr << "Failed test at " << file << "(" << line << "): " << a << " != " << b << std::endl;
        RegisterFail();
    }
}

#define TEST_EQUALS(A, B) TestEquals(A, B, __FILE__, __LINE__);
