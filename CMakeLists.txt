cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_COMPILER /usr/bin/gcc-13)
set(CMAKE_CXX_COMPILER /usr/bin/g++-13)
set(CMAKE_LINKER /usr/bin/ldd)

project(slam_dunk)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-linux")

set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "")

find_package(glbinding CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)


add_library(slam_dunk)
target_sources(
    slam_dunk

    PUBLIC
        include/public/slam_dunk/slam_dunk.hpp
        include/public/slam_dunk/slamvas.hpp

    PRIVATE
        src/slamvas.cpp

        include/private/slam_dunk/glfw.hpp
        src/glfw.cpp

        include/private/slam_dunk/tree.hpp

        include/private/slam_dunk/node.hpp
)

target_include_directories(
    slam_dunk 

    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/public
    
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/private
)

target_link_libraries(
    slam_dunk 
    
    PUBLIC

    glbinding::glbinding
    glbinding::glbinding-aux

    spdlog::spdlog

    glm::glm

    glfw
)

add_subdirectory(examples)
