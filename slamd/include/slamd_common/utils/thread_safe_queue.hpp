#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

namespace slamd {
namespace _utils {

template <typename T>
class ThreadSafeQueue {
   public:
    ThreadSafeQueue() {}

    void push(
        const T& value
    ) {
        std::scoped_lock l(this->mutex);

        this->queue.push(value);
        this->cond.notify_one();
    }

    void push(
        T&& value
    ) {
        std::scoped_lock l(this->mutex);

        this->queue.push(std::move(value));
        this->cond.notify_one();
    }

    std::optional<T> try_pop() {
        std::scoped_lock l(this->mutex);

        if (this->queue.empty()) {
            return std::nullopt;
        }

        T element = std::move(this->queue.front());
        this->queue.pop();

        return element;
    }

    T block_pop() {
        std::unique_lock<std::mutex> lock(this->mutex);

        this->cond.wait(this->mutex, [this] {
            return !this->queue.empty();
        });

        T item = this->queue.front();
        this->queue.pop();
        return item;
    }

    std::optional<T> timeout_pop(
        std::chrono::milliseconds timeout
    ) {
        std::unique_lock<std::mutex> lock(this->mutex);

        if (!this->cond.wait_for(lock, timeout, [this] {
                return !this->queue.empty();
            })) {
            return std::nullopt;
        }

        T item = std::move(this->queue.front());
        this->queue.pop();
        return item;
    }

   private:
    std::condition_variable cond;
    std::mutex mutex;
    std::queue<T> queue;
};

}  // namespace _utils
}  // namespace slamd