#include <spdlog/spdlog.h>
#include <asio.hpp>
#include <iostream>
#include <slamd_window/window.hpp>

int main() {
    spdlog::set_level(spdlog::level::debug);

    asio::io_context io;
    asio::ip::tcp::socket socket(io);

    socket.connect(
        asio::ip::tcp::endpoint(asio::ip::make_address("127.0.0.1"), 5555)
    );

    // read initial state

    while (true) {
        uint32_t len_net;
        asio::read(socket, asio::buffer(&len_net, 4));
        uint32_t len = ntohl(len_net);

                std::vector<char> buf(len);
        asio::read(socket, asio::buffer(buf.data(), len));

        std::string msg(buf.begin(), buf.end());
        std::cout << "💡 Update: " << msg << "\n";
    }
}