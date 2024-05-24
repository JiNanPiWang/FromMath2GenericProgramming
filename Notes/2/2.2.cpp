//
// Created by Samsara on 2024/5/23.
// File name: 2.cpp
// Description: 递归与迭代
//
#include <iostream>
#include <chrono>
#include <functional>
#include <map>
#include <format>

using namespace std;
using namespace std::chrono;

// TODO: 待改成模板类
// 递归计算乘法（类似快速幂）
int multiply1(int n, int a)
{
    if (n == 1)
        return a;
    int result = multiply1(n / 2, a + a);
    if (n % 2 == 1)
        result += a;
    return result;
}

// 找到一个不变式，使得递归变得“通用”
// 我们乘法实际上可以简化为计算r + na，比如6 * 9 = 0 + 3 * 18 = 18 + 1 * 36
// 不变式：r + na = r0 + n0a0
int mult_acc0(int r, int n, int a)
{
    if (n == 1)
        return r + a;
    if (n % 2 == 1)
        return mult_acc0(r + a, n / 2, a + a);
    return mult_acc0(r, n / 2, a + a);
}

// 但是上面有两个递归，我们可以将其合并
int mult_acc1(int r, int n, int a)
{
    if (n == 1)
        return r + a;
    if (n % 2 == 1)
       r += a;
    return mult_acc1(r, n / 2, a + a);
}

// 再度优化，提前处理n和a
int mult_acc3(int r, int n, int a)
{
    if (n % 2 == 1)
    {
        r += a;
        if (n == 1)
            return r;
    }
    n /= 2;
    a += a;
    return mult_acc3(r, n, a);
}

// 这样就很简单转化为迭代了
int mult_acc4(int r, int n, int a)
{
    while (true)
    {
        if (n % 2 == 1)
        {
            r += a;
            if (n == 1)
                return r;
        }
        n /= 2;
        a += a;
    }
}

// 定义测试函数
template<typename Func, typename... Args>
long long test_function(Func func, Args&&... args) {
    auto start = high_resolution_clock::now();
    func(std::forward<Args>(args)...); // 调用待测试的函数
    auto end = high_resolution_clock::now();
    return duration_cast<nanoseconds>(end - start).count(); // 返回执行时间（纳秒）
}

int main() {
    // 定义函数指针映射，multiply1仅接收两个参数，较麻烦，所以不在测试范围内
    map<string, function<int(int, int, int)>> functions = {
//            {"multiply1", multiply1},
            {"mult_acc0", mult_acc0},
            {"mult_acc1", mult_acc1},
            {"mult_acc3", mult_acc3},
            {"mult_acc4", mult_acc4}
    };

    // 待测试的参数
    int n = 1234567;
    int a = 123;

    cout << format("Computing {} * {} using different functions", n, a) << endl;

    // 测试每个函数的效率
    for (const auto& pair : functions) {
        cout << "Function " << pair.first;
        long long time = test_function(pair.second, 0, n, a); // 测试函数执行时间
        cout << " taken: " << time << " nanoseconds to compute" << endl;
    }

    return 0;
}