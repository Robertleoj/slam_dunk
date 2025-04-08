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

        include/slamd/scene_object.hpp
        src/scene_object.cpp

        include/slamd/scene_objects/box.hpp
        src/scene_objects/box.cpp

        include/slamd/scene_objects/simple_mesh.hpp
        src/scene_objects/simple_mesh.cpp

        include/slamd/scene_objects/arrows.hpp
        src/scene_objects/arrows.cpp

        include/slamd/scene_objects/sphere.hpp
        src/scene_objects/sphere.cpp

        include/slamd/scene_objects/triad.hpp
        src/scene_objects/triad.cpp

        include/slamd/scene_objects/mono_mesh.hpp
        src/scene_objects/mono_mesh.cpp

        include/slamd/scene_objects/poly_line.hpp
        src/scene_objects/poly_line.cpp

        include/slamd/scene_objects/xy_grid.hpp
        src/scene_objects/xy_grid.cpp

        include/slamd/scene_objects/arcball_indicator.hpp
        src/scene_objects/arcball_indicator.cpp

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
