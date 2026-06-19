#include <iostream>
#include <atomic>
#include <cassert>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(size_t n) : stop(false) {
        for (size_t i = 0; i < n; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }
    void enqueue(std::function<void()> task) {
        { std::unique_lock<std::mutex> lock(queue_mutex); tasks.push(std::move(task)); }
        condition.notify_one();
    }
    ~ThreadPool() {
        { std::unique_lock<std::mutex> lock(queue_mutex); stop = true; }
        condition.notify_all();
        for (auto& w : workers) w.join();
    }
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop;
};

int main() {
    ThreadPool pool(2);
    std::atomic<int> count{0};
    for (int i = 0; i < 10; ++i) pool.enqueue([&count] { count++; });
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    assert(count.load() == 10);
    std::cout << "All thread pool tests passed!\n";
    return 0;
}
