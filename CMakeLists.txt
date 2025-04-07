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


add_library(slam_dunk)
target_sources(
    slam_dunk

    PUBLIC
        include/slam_dunk/slam_dunk.hpp

        include/slam_dunk/scene.hpp
        src/scene.cpp

        include/slam_dunk/glfw.hpp
        src/glfw.cpp

        include/slam_dunk/tree.hpp
        src/tree.cpp

        include/slam_dunk/node.hpp
        src/node.cpp

        include/slam_dunk/frame_buffer.hpp
        src/frame_buffer.cpp

        include/slam_dunk/tree_path.hpp
        src/tree_path.cpp

        include/slam_dunk/scene_object.hpp
        src/scene_object.cpp

        include/slam_dunk/scene_objects/box.hpp
        src/scene_objects/box.cpp

        include/slam_dunk/scene_objects/simple_mesh.hpp
        src/scene_objects/simple_mesh.cpp

        include/slam_dunk/scene_objects/arrows.hpp
        src/scene_objects/arrows.cpp

        include/slam_dunk/scene_objects/sphere.hpp
        src/scene_objects/sphere.cpp

        include/slam_dunk/scene_objects/triad.hpp
        src/scene_objects/triad.cpp

        include/slam_dunk/scene_objects/mono_mesh.hpp
        src/scene_objects/mono_mesh.cpp

        include/slam_dunk/scene_objects/poly_line.hpp
        src/scene_objects/poly_line.cpp

        include/slam_dunk/scene_objects/xy_grid.hpp
        src/scene_objects/xy_grid.cpp

        include/slam_dunk/camera.hpp
        src/camera.cpp

        include/slam_dunk/io.hpp
        src/io.cpp

        include/slam_dunk/shaders.hpp
        src/shaders.cpp

        include/slam_dunk/paths.hpp

        include/slam_dunk/angle.hpp
        src/angle.cpp

        include/slam_dunk/arcball.hpp
        src/arcball.cpp

        include/slam_dunk/transforms.hpp
        src/transforms.cpp

        include/slam_dunk/frame_timer.hpp
        src/frame_timer.cpp

)

target_include_directories(
    slam_dunk 

    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    
)

target_link_libraries(
    slam_dunk 
    
    PUBLIC

    glbinding::glbinding
    glbinding::glbinding-aux

    spdlog::spdlog

    glm::glm

    glfw

    imgui::imgui
)

add_subdirectory(examples)
