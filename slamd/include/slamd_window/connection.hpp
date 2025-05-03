#pragma once

#include <asio.hpp>
#include <slamd_common/utils/thread_safe_queue.hpp>
#include <slamd_window/message.hpp>
#include <thread>

namespace slamd {

class Connection {
   public:
    Connection(std::string ip, ushort port);

   public:
    const std::string ip;
    const ushort port;
    bool connected = false;
    _utils::ThreadSafeQueue<std::unique_ptr<Message>> messages;

   private:
    void job();

    std::jthread job_thread;
};

}  // namespace slamd
