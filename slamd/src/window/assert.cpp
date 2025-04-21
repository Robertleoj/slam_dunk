#include <slamd/assert.hpp>
#include <stdexcept>

namespace slamd {
void assert_thread(
    std::thread::id thread_id
) {
    if (std::this_thread::get_id() != thread_id) {
        throw std::runtime_error("Thread assert failed");
    }
}
}  // namespace slamd