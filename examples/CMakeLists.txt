cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(slam_dunk_examples)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(APPLE)
    message(STATUS "APPLE")
    set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/arm64-osx")
elseif(UNIX)
    message(STATUS "UNIX")
    set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-linux")
endif()

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

find_path(Stb_INCLUDE_DIR "stb_image.h")

# find_package(spdlog CONFIG REQUIRED)

# to use as submodule
add_subdirectory(slamd)

# to use with fetchcontent
# include(FetchContent)

# FetchContent_Declare(
#   slamd
#   GIT_REPOSITORY https://github.com/Robertleoj/slam_dunk.git
#   GIT_TAG main
#   SOURCE_SUBDIR slamd
# )

# FetchContent_MakeAvailable(slamd)

add_subdirectory(hello_world)
add_subdirectory(two_scenes)
add_subdirectory(one_scene_two_views)
add_subdirectory(arrows)
add_subdirectory(poly_line)
add_subdirectory(poly_line_2d)
add_subdirectory(mesh)
add_subdirectory(moving_box)
add_subdirectory(hello_canvas)
add_subdirectory(point_cloud)
add_subdirectory(camera_frustum)
add_subdirectory(circles)
add_subdirectory(hello_visualizer)
add_subdirectory(three_d_spiral)