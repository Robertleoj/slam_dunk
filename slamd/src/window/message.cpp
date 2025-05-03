#include <slamd_window/message.hpp>

namespace slamd {

Message::Message(
    size_t bytes
) {
    this->buffer = std::unique_ptr<uint8_t[]>(new uint8_t[bytes]);
}

// Message::Message(
//     Message&& other
// ) {
//     this->buffer = std::move(other.buffer);
// }

uint8_t* Message::data() {
    return this->buffer.get();
}

const slamd::flatb::Message* Message::msg() {
    return slamd::flatb::GetMessage(this->data());
}

}  // namespace slamd