#pragma once
#include <atomic>
#include <cstdint>

namespace slamd {
namespace _id {

template <typename Tag>
struct ID {
    const uint64_t value;

    static std::atomic<uint64_t> counter;  // Declare static member

    explicit ID(
        uint64_t v
    )
        : value(v) {}  // Manual ID creation

    static ID next() { return ID(counter++); }  // Generate unique ID

    bool operator==(
        const ID& other
    ) const {
        return value == other.value;
    }
    bool operator!=(
        const ID& other
    ) const {
        return value != other.value;
    }
    bool operator<(
        const ID& other
    ) const {
        return value < other.value;
    }
};

// Define static counter outside the struct
template <typename Tag>
std::atomic<uint64_t> ID<Tag>::counter{1};

}  // namespace _id
}  // namespace slamd

namespace std {
template <typename Tag>
struct hash<slamd::_id::ID<Tag>> {
    size_t operator()(
        const slamd::_id::ID<Tag>& id
    ) const {
        return hash<uint64_t>()(id.value);
    }
};
}  // namespace std