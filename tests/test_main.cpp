#include "sky/thread_pool.hpp"

#include <future>
#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    {
        sky::ThreadPool pool(3, 8);
        std::vector<std::future<int>> results;
        for (int i = 0; i < 50; ++i) {
            results.push_back(pool.submit([i] { return i + 1; }));
        }
        long sum = 0;
        for (auto& result : results) {
            sum += result.get();
        }
        pool.wait_idle();
        if (sum != 1275 || pool.queued() != 0 || pool.worker_count() != 3) {
            std::cerr << "execution invariant failed\n";
            return 1;
        }
    }

    {
        sky::ThreadPool pool(2, 2);
        auto failure = pool.submit([]() -> int { throw std::runtime_error("task failure"); });
        bool propagated = false;
        try {
            (void)failure.get();
        } catch (const std::runtime_error&) {
            propagated = true;
        }
        if (!propagated) {
            std::cerr << "task exception was not propagated\n";
            return 1;
        }
    }

    {
        bool rejected = false;
        try {
            sky::ThreadPool invalid(0);
        } catch (const std::invalid_argument&) {
            rejected = true;
        }
        if (!rejected) {
            std::cerr << "zero-worker configuration was accepted\n";
            return 1;
        }
    }

    std::cout << "Sky Thread Pool tests passed\n";
    return 0;
}
