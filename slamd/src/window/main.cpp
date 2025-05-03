#include <flatb/visualizer_generated.h>
#include <spdlog/spdlog.h>
#include <asio.hpp>
#include <cxxopts.hpp>
#include <iostream>
#include <slamd_window/window.hpp>

int main(
    int argc,
    char* argv[]
) {
    spdlog::set_level(spdlog::level::debug);

    cxxopts::Options options(
        "SlamDunk Window",
        "The viewer for the SlamDunk library"
    );

    options
        .add_options()
        ("w,window-name", "Name of the window (required)", cxxopts::value<std::string>())
        ("p,port", "Port number to use", cxxopts::value<int>()->default_value("5555"))
        (
            "h,help",
            "Show help"
        );

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (!result.count("window-name")) {
        std::cerr
            << "Bro, you forgot the --window-name arg. Mandatory, my dude.\n";
        std::cout << options.help() << std::endl;
        return 1;
    }

    std::string name = result["window-name"].as<std::string>();
    uint16_t port = result["port"].as<int>();

    slamd::Window window{name};
    window.state_manager.try_connect("127.0.0.1", port);

    window.run();
}