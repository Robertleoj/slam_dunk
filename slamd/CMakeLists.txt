cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_COMPILER /usr/bin/gcc-13)
set(CMAKE_CXX_COMPILER /usr/bin/g++-13)
set(CMAKE_LINKER /usr/bin/ldd)

project(slam_dunk VERSION 0.1.0)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(glbinding CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_library(slamd)
add_library(slamd::slamd ALIAS slamd)
target_sources(
    slamd

    PRIVATE
        src/camera_2d.cpp
        src/image_texture.cpp
        src/glfw.cpp
        src/frame_buffer.cpp
        src/camera.cpp
        src/io.cpp
        src/shaders.cpp
        src/arcball.cpp
        src/frame_timer.cpp
        src/assert.cpp
        src/window.cpp
        src/render_thread_job_queue.cpp

        src/data/image.cpp

        src/tree/tree.cpp
        src/tree/node.cpp
        src/tree/tree_path.cpp

        src/view/scene_view.cpp
        src/view/canvas_view.cpp

        src/gmath/aabb.cpp
        src/gmath/rect2d.cpp
        src/gmath/angle.cpp
        src/gmath/transforms.cpp

        src/geometry/geometry.cpp
        src/geometry/box.cpp
        src/geometry/simple_mesh.cpp
        src/geometry/arrows.cpp
        src/geometry/sphere.cpp
        src/geometry/triad.cpp
        src/geometry/mono_mesh.cpp
        src/geometry/poly_line.cpp
        src/geometry/xy_grid.cpp
        src/geometry/arcball_indicator.cpp
        src/geometry/image.cpp
)

target_include_directories(
    slamd

    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    slamd 
    
    PRIVATE

        glbinding::glbinding
        glbinding::glbinding-aux

        spdlog::spdlog

        glfw

        imgui::imgui

    PUBLIC
        glm::glm
)

install(TARGETS slamd
    EXPORT slamdTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/slamdConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/slamdConfig.cmake"
    INSTALL_DESTINATION share/slamd
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/slamdConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/slamdConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/slamdConfigVersion.cmake"
    DESTINATION share/slamd
)

install(EXPORT slamdTargets
    FILE slamdTargets.cmake
    NAMESPACE slamd::
    DESTINATION share/slamd
)

