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
find_package(imgui CONFIG REQUIRED)


add_library(slamd)
target_sources(
    slamd

    PUBLIC
        include/slamd/slamd.hpp

        include/slamd/scene.hpp
        src/scene.cpp

        include/slamd/glfw.hpp
        src/glfw.cpp

        include/slamd/tree.hpp
        src/tree.cpp

        include/slamd/node.hpp
        src/node.cpp

        include/slamd/frame_buffer.hpp
        src/frame_buffer.cpp

        include/slamd/tree_path.hpp
        src/tree_path.cpp

        include/slamd/geometry/geometry.hpp
        src/geometry/geometry.cpp

        include/slamd/geometry/box.hpp
        src/geometry/box.cpp

        include/slamd/geometry/simple_mesh.hpp
        src/geometry/simple_mesh.cpp

        include/slamd/geometry/arrows.hpp
        src/geometry/arrows.cpp

        include/slamd/geometry/sphere.hpp
        src/geometry/sphere.cpp

        include/slamd/geometry/triad.hpp
        src/geometry/triad.cpp

        include/slamd/geometry/mono_mesh.hpp
        src/geometry/mono_mesh.cpp

        include/slamd/geometry/poly_line.hpp
        src/geometry/poly_line.cpp

        include/slamd/geometry/xy_grid.hpp
        src/geometry/xy_grid.cpp

        include/slamd/geometry/arcball_indicator.hpp
        src/geometry/arcball_indicator.cpp

        include/slamd/camera.hpp
        src/camera.cpp

        include/slamd/io.hpp
        src/io.cpp

        include/slamd/shaders.hpp
        src/shaders.cpp

        include/slamd/paths.hpp

        include/slamd/angle.hpp
        src/angle.cpp

        include/slamd/arcball.hpp
        src/arcball.cpp

        include/slamd/transforms.hpp
        src/transforms.cpp

        include/slamd/frame_timer.hpp
        src/frame_timer.cpp

        include/slamd/assert.hpp
        src/assert.cpp

        include/slamd/render_thread_job_queue.hpp

        include/slamd/thread_box.hpp
)

target_include_directories(
    slamd 

    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    
)

target_link_libraries(
    slamd 
    
    PUBLIC

    glbinding::glbinding
    glbinding::glbinding-aux

    spdlog::spdlog

    glm::glm

    glfw

    imgui::imgui
)

add_subdirectory(examples)
