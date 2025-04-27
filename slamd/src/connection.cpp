#include <spdlog/spdlog.h>
#include <iostream>
#include <slamd/connection.hpp>

namespace slamd {

Connection::Connection(
    asio::ip::tcp::socket socket
)
    : socket(std::move(socket)) {
    SPDLOG_INFO("Client connected");
}

void Connection::write(
    const std::vector<uint8_t>& msg
) {
    auto shared_msg = std::make_shared<std::vector<uint8_t>>(msg);
    auto shared_len = std::make_shared<uint32_t>(htonl(msg.size()));

    std::vector<asio::const_buffer> buffers = {
        asio::buffer(shared_len.get(), sizeof(uint32_t)),
        asio::buffer(*shared_msg)
    };

    asio::async_write(
        socket,
        buffers,
        [self = shared_from_this(),
         shared_msg,
         shared_len](std::error_code ec, std::size_t) {
            if (ec) {
                std::cerr << "Write failed: " << ec.message() << "\n";
            }
            // shared_msg and shared_len stay alive until here
        }
    );
}

}  // namespace slamd