add_executable(hello_canvas main.cpp)

target_link_libraries(
    hello_canvas PRIVATE

    slamd::slamd
    spdlog::spdlog
)

target_include_directories(
    hello_canvas 
    
    PRIVATE
    ${Stb_INCLUDE_DIR}
)