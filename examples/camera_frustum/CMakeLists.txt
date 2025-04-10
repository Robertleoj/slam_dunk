add_executable(camera_frustum main.cpp)

target_link_libraries(
    camera_frustum PRIVATE

    slamd::slamd
)