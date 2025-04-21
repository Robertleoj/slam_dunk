#pragma once

#include <asio.hpp>
#include <slamd_window/message_queue.hpp>

namespace slamdw {

class Connection {
   public:
    Connection(std::string ip, ushort port);

   public:
    const std::string ip;
    const ushort port;
    bool connected = false;
    MessageQueue<std::vector<uint8_t>> messages;

   private:
    void job();

    std::jthread job_thread;
};

}  // namespace slamdw
