#pragma once
#include <asio.hpp>
#include <cstdint>
#include <memory>
#include <slamd_common/utils/thread_safe_queue.hpp>
#include <stop_token>
#include <thread>

namespace slamd {
namespace _net {

class Connection : public std::enable_shared_from_this<Connection> {
   public:
    Connection(asio::ip::tcp::socket socket);
    void write(const std::vector<uint8_t>& msg);
    void write(std::shared_ptr<std::vector<uint8_t>> msg);
    bool is_alive();

   private:
    std::jthread worker;
    void job(std::stop_token& stop_token);

    bool alive;
    asio::ip::tcp::socket socket;
    _utils::ThreadSafeQueue<std::shared_ptr<std::vector<uint8_t>>>
        message_queue;
};

}  // namespace _net
}  // namespace slamd