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

template <typename T, typename FUNC>
void TestEquals(const T& a, const T& b, FUNC equalFunc, const char* file, int line)
{
    if (!equalFunc(a, b))
    {
        std::cerr << "Failed test at " << file << "(" << line << "): " << a << " != " << b << std::endl;
        RegisterFail();
    }
}

template <typename T>
void TestEquals(const T& a, const T& b, const char* file, int line)
{
    auto equalFunc = [](const T& a, const T& b) { return a == b; };
    TestEquals<T>(a, b, equalFunc, file, line);
}

#define TEST_EQUALS(A, B) TestEquals(A, B, __FILE__, __LINE__);
#define TEST_EQUALS_FUNC(A, B, FUNC) TestEquals(A, B, FUNC, __FILE__, __LINE__);
#define REGISTER_TEST(test)                                                                                            \
    void func_##test();                                                                                                \
    RegisteredTest test_##test(func_##test);                                                                           \
    void func_##test()
