cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_COMPILER /usr/bin/gcc-13)
set(CMAKE_CXX_COMPILER /usr/bin/g++-13)
set(CMAKE_LINKER /usr/bin/ldd)

project(slam_dunk_examples)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)


set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-linux")

find_package(spdlog CONFIG REQUIRED)

add_subdirectory(slamd)

add_subdirectory(hello_world)
add_subdirectory(two_scenes)
add_subdirectory(one_scene_two_views)
add_subdirectory(moving_box)
add_subdirectory(hello_canvas)
add_subdirectory(point_cloud)
add_subdirectory(camera_frustum)