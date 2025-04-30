#pragma once

#include <mutex>
#include <queue>
#include <thread>

namespace slamd {

template <typename T>
class MessageQueue {
   public:
    MessageQueue() {}

    void push(
        const T& value
    ) {
        std::scoped_lock l(this->mutex);

        this->queue.push(value);
    }

    void push(
        T&& value
    ) {
        std::scoped_lock l(this->mutex);

        this->queue.push(std::move(value));
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

   private:
    std::mutex mutex;
    std::queue<T> queue;
};

}  // namespace slamd