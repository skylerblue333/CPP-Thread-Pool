#include "sky/thread_pool.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <future>
#include <stdexcept>
#include <thread>
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
        assert(sum == 1275);
        pool.wait_idle();
        assert(pool.queued() == 0);
        assert(pool.worker_count() == 3);
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
        assert(propagated);
    }

    {
        bool rejected = false;
        try {
            sky::ThreadPool invalid(0);
        } catch (const std::invalid_argument&) {
            rejected = true;
        }
        assert(rejected);
    }

    return 0;
}
