add_executable(point_cloud main.cpp)

target_link_libraries(
    point_cloud PRIVATE

    slamd::slamd
)