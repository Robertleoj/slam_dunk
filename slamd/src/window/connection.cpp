#include <asio.hpp>
#include <slamd_window/connection.hpp>
#include <thread>
#include <spdlog/spdlog.h>

namespace slamdw {

Connection::Connection(
    std::string ip,
    ushort port
)
    : ip(ip),
      port(port) {
    this->job_thread = std::jthread(&Connection::job, this);
}


void Connection::job() {
    spdlog::info("Connection job started for {}:{}", ip, port);

    asio::io_context io_ctx;
    asio::ip::tcp::socket socket(io_ctx);
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address(ip), port);

    while (true) {
        try {
            socket.connect(endpoint);
            connected = true;
            spdlog::info("Successfully connected to {}:{}", ip, port);
            break;
        } catch (const std::exception& e) {
            connected = false;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    while (connected) {
        try {
            // === Placeholder: slap in your message reading here ===
            // std::vector<uint8_t> msg = read_message(socket);
            // messages.push(msg);
            spdlog::debug("Waiting for message...");

        } catch (const std::exception& e) {
            spdlog::error("Error while reading from socket: {}", e.what());
            connected = false;
            socket.close();
            spdlog::info("Socket closed due to error.");
            break;
        }
    }

    spdlog::info("Connection job ended for {}:{}", ip, port);
}

}  // namespace slamdw