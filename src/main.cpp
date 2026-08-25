#include "sky/thread_pool.hpp"

#include <iostream>
#include <numeric>
#include <vector>

int main() {
    sky::ThreadPool pool(4, 64);
    std::vector<std::future<int>> futures;
    futures.reserve(20);

    for (int i = 1; i <= 20; ++i) {
        futures.push_back(pool.submit([i] { return i * i; }));
    }

    int total = 0;
    for (auto& future : futures) {
        total += future.get();
    }
    pool.wait_idle();

    std::cout << "sky-thread-pool completed 20 tasks; checksum=" << total << '\n';
    return total == 2870 ? 0 : 1;
}
