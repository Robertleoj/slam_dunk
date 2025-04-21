#include <spdlog/spdlog.h>
#include <iostream>
#include <slamd_window/connection.hpp>

namespace slamdw {

Connection::Connection(
    asio::ip::tcp::socket socket
)
    : socket(std::move(socket)) {
    SPDLOG_INFO("Client connected");

    this->send_initial_state();
}

void Connection::send_initial_state() {
    // TODO: implement
}

void Connection::write(
    const std::vector<uint8_t>& msg
) {
    uint32_t len = htonl(msg.size());
    std::vector<asio::const_buffer> buffers = {
        asio::buffer(&len, sizeof(len)),
        asio::buffer(msg)
    };

    asio::async_write(
        socket,
        buffers,
        [self = shared_from_this()](std::error_code ec, std::size_t) {
            if (ec) {
                std::cerr << "Write failed: " << ec.message() << "\n";
            }
        }
    );
}

}  // namespace slamd