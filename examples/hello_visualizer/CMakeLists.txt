add_executable(hello_visualizer main.cpp)

target_link_libraries(
    hello_visualizer
    PRIVATE

    slamd::slamd
    spdlog::spdlog
)