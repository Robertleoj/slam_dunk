#pragma once
#include <flatb/messages_generated.h>
#include <cstdint>
#include <memory>

namespace slamd {

class Message {
   public:
    Message(size_t bytes);

    uint8_t* data();
    const slamd::flatb::Message* msg();

   private:
    std::unique_ptr<uint8_t[]> buffer;
};

}  // namespace slamd