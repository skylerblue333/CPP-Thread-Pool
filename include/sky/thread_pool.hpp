#pragma once

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace sky {

class ThreadPool {
public:
    explicit ThreadPool(std::size_t workers, std::size_t max_queue = 1024)
        : max_queue_(max_queue) {
        if (workers == 0) {
            throw std::invalid_argument("workers must be greater than zero");
        }
        if (max_queue == 0) {
            throw std::invalid_argument("max_queue must be greater than zero");
        }

        workers_.reserve(workers);
        for (std::size_t i = 0; i < workers; ++i) {
            workers_.emplace_back([this] { worker_loop(); });
        }
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    ~ThreadPool() { shutdown(); }

    template <class Fn, class... Args>
    auto submit(Fn&& fn, Args&&... args)
        -> std::future<std::invoke_result_t<Fn, Args...>> {
        using Result = std::invoke_result_t<Fn, Args...>;
        auto task = std::make_shared<std::packaged_task<Result()>>(
            std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...));
        auto future = task->get_future();

        {
            std::unique_lock<std::mutex> lock(mutex_);
            queue_space_.wait(lock, [this] {
                return stopping_ || tasks_.size() < max_queue_;
            });
            if (stopping_) {
                throw std::runtime_error("thread pool is shutting down");
            }
            tasks_.emplace([task] { (*task)(); });
        }
        work_available_.notify_one();
        return future;
    }

    void wait_idle() {
        std::unique_lock<std::mutex> lock(mutex_);
        idle_.wait(lock, [this] { return tasks_.empty() && active_ == 0; });
    }

    void shutdown() noexcept {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (stopping_) {
                return;
            }
            stopping_ = true;
        }
        work_available_.notify_all();
        queue_space_.notify_all();
        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    [[nodiscard]] std::size_t worker_count() const noexcept {
        return workers_.size();
    }

    [[nodiscard]] std::size_t queued() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return tasks_.size();
    }

private:
    void worker_loop() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                work_available_.wait(lock, [this] {
                    return stopping_ || !tasks_.empty();
                });
                if (stopping_ && tasks_.empty()) {
                    return;
                }
                task = std::move(tasks_.front());
                tasks_.pop();
                ++active_;
            }
            queue_space_.notify_one();

            task();

            {
                std::lock_guard<std::mutex> lock(mutex_);
                --active_;
                if (tasks_.empty() && active_ == 0) {
                    idle_.notify_all();
                }
            }
        }
    }

    const std::size_t max_queue_;
    mutable std::mutex mutex_;
    std::condition_variable work_available_;
    std::condition_variable queue_space_;
    std::condition_variable idle_;
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::size_t active_{0};
    bool stopping_{false};
};

} // namespace sky
