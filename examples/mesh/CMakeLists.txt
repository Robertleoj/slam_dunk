add_executable(mesh main.cpp)

target_link_libraries(
    mesh PRIVATE

    slamd::slamd
)