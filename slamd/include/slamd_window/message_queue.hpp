#pragma once

#include <atomic>
#include <optional>
#include <vector>

namespace slamd {

// TODO: use linked list, not circular buffer to avoid capacity limit
template <typename T>
class MessageQueue {
   public:
    explicit MessageQueue(
        size_t capacity
    )
        : buffer(capacity),
          capacity(capacity),
          head(0),
          tail(0) {}

    bool push(
        const T& value
    ) {
        size_t current_tail = tail.load(std::memory_order_relaxed);
        size_t next_tail = (current_tail + 1) % capacity;

        if (next_tail == head.load(std::memory_order_acquire)) {
            return false;  // Queue full
        }

        buffer[current_tail] = value;
        tail.store(next_tail, std::memory_order_release);
        return true;
    }

    bool push(
        T&& value
    ) {
        size_t current_tail = tail.load(std::memory_order_relaxed);
        size_t next_tail = (current_tail + 1) % capacity;

        if (next_tail == head.load(std::memory_order_acquire)) {
            return false;  // Queue full
        }

        buffer[current_tail] = std::move(value);
        tail.store(next_tail, std::memory_order_release);
        return true;
    }

    std::optional<T> try_pop() {
        size_t current_head = head.load(std::memory_order_relaxed);

        if (current_head == tail.load(std::memory_order_acquire)) {
            return std::nullopt;  // Queue empty
        }

        T value = std::move(buffer[current_head]);
        head.store((current_head + 1) % capacity, std::memory_order_release);
        return value;
    }

   private:
    std::vector<T> buffer;
    const size_t capacity;
    std::atomic<size_t> head;
    std::atomic<size_t> tail;
};

}  // namespace slamd
