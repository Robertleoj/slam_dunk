#pragma once
#include <asio.hpp>
#include <cstdint>
#include <memory>
#include <string>

namespace slamdw {

class Connection : public std::enable_shared_from_this<Connection> {
   public:
    Connection(asio::ip::tcp::socket socket);

   private:
    void send_initial_state();
    void write(const std::vector<uint8_t>& msg);

    asio::ip::tcp::socket socket;
};

}