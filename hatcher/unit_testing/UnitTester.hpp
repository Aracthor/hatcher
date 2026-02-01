#pragma once

#include <functional>
#include <iostream>

struct RegisteredTest
{
    RegisteredTest(std::function<void()> test);

    std::function<void()> test;
    const RegisteredTest* next;

    static RegisteredTest* s_first;
};

void RegisterFail();

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
#define REGISTER_TEST(test)                                                                                            \
    void func_##test();                                                                                                \
    RegisteredTest test_##test(func_##test);                                                                           \
    void func_##test()
