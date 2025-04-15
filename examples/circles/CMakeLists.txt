add_executable(circles main.cpp)

target_link_libraries(
    circles PRIVATE

    slamd::slamd
)