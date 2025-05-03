add_executable(arrows main.cpp)
target_link_libraries(
    arrows 
    PRIVATE

    slamd::slamd
    spdlog::spdlog
)

target_include_directories(
    arrows PRIVATE
)