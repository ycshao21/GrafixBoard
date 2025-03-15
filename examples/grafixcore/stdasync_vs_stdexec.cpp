#include <atomic>
#include <chrono>
#include <cmath>
#include <exec/static_thread_pool.hpp>
#include <future>
#include <iostream>
#include <spdlog/spdlog.h>
#include <stdexec/execution.hpp>
#include <vector>

// Compute-bound functions
auto heavyComputation1(int n) -> int
{
    int result = 0;
    for (int i = 0; i < n * 10000000; ++i) {
        result += (i % 17 == 0) ? i : -i;
    }
    return result;
}

auto heavyComputation2(double x) -> int
{
    double result = 0.0;
    for (int i = 0; i < 15000000; ++i) {
        result += std::sin(i * x) * std::cos(i * 0.001) * std::tan(i * 0.00001);
    }
    return int(result);
}

auto heavyComputation3(int size) -> int
{
    std::vector<int> result(size);
    for (int i = 0; i < size; ++i) {
        int sum = 0;
        for (int j = 0; j < 5000000; ++j) {
            sum += (j * i) % 123 * (j % 3);
            if (sum % 11 == 0) {
                sum = std::max(sum, j % 17);
            }
        }
        result[i] = sum;
    }
    return result.size();
}

auto stdexecTest() -> int
{
    spdlog::info("stdexecTest1");

    auto start = std::chrono::high_resolution_clock::now();

    auto threadPool =
        exec::static_thread_pool(std::thread::hardware_concurrency());
    auto sched = threadPool.get_scheduler();
    (void) sched;

    // I dont know how to submit tasks to ::exec thread pool.

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    spdlog::info("Duration: {} ms", duration.count());

    return 0;
}

auto stdasyncTest()
{
    spdlog::info("stdasyncTest1");

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::future<int>> futures;

    for (int i = 0; i < 10; ++i) {
        futures.push_back(
            std::async(std::launch::async, heavyComputation1, 100));
        futures.push_back(
            std::async(std::launch::async, heavyComputation2, 0.1));
        futures.push_back(
            std::async(std::launch::async, heavyComputation3, 100));
    }

    for (auto& f : futures) {
        f.wait();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    spdlog::info("Duration: {} ms", duration.count());

    return 0;
}

auto main() -> int
{
    spdlog::set_level(spdlog::level::info);

    stdexecTest();
    stdasyncTest();

    return 0;
}