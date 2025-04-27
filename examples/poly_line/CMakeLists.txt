add_executable(poly_line main.cpp)

target_link_libraries(
    poly_line PRIVATE

    slamd::slamd
)