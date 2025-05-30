cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SLAMD_ENABLE_INSTALL "Enable install targets" OFF)
option(SLAMD_VENDOR_DEPS "Use vendored dependencies" ON)

project(slam_dunk VERSION 0.1.0)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LIB_DIR vendored_deps)

function(find_required_package 
    pkg_name 
    target_name 
    found_var 
    subdir
)
    if (NOT TARGET ${target_name})
        if (SLAMD_VENDOR_DEPS)
            message(STATUS "Vendoring is ON, skipping find_package for ${pkg_name}")
            if (NOT TARGET ${target_name})
                message(STATUS "${pkg_name} target not found, trying add_subdirectory")
                add_subdirectory(${subdir})
            endif()
        else()
            message(STATUS "Finding ${pkg_name}")
            find_package(${pkg_name} CONFIG QUIET)
        endif()

        if (NOT TARGET ${target_name})
            message(FATAL_ERROR "${pkg_name} still not found. No target, no backup — we're hosed.")
        endif()
    endif()
endfunction()

# sometimes glbinding-aux does not come with
# glbinding, so we need to check for that target
find_required_package(
    glbinding
    glbinding::glbinding-aux
    glbinding_FOUND
    ${LIB_DIR}/glbinding
)

find_required_package(
    glfw
    glfw
    glfw_FOUND
    ${LIB_DIR}/glfw
) 

find_required_package(
    spdlog 
    spdlog::spdlog 
    spdlog_FOUND
    ${LIB_DIR}/spdlog
)

find_required_package(
    imgui 
    imgui::imgui
    imgui_FOUND
    ${LIB_DIR}/imgui_wrapper
)

find_required_package(
    fmt
    fmt::fmt
    fmt_FOUND
    ${LIB_DIR}/fmt
)

add_subdirectory(${LIB_DIR}/glm)
add_subdirectory(${LIB_DIR}/cxxopts)

add_subdirectory(flatb)


# COMMON
add_library(slamd_common STATIC)
target_sources(
    slamd_common

    PRIVATE
        src/common/assert.cpp

        src/common/data/image.cpp
        src/common/data/mesh.cpp

        src/common/utils/mesh.cpp

        src/common/gmath/aabb.cpp
        src/common/gmath/rect2d.cpp
        src/common/gmath/angle.cpp
        src/common/gmath/transforms.cpp
        src/common/gmath/serialization.cpp
)

target_include_directories(
    slamd_common

    PUBLIC
        ${LIB_DIR}/glm
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_options(slamd_common PRIVATE -Wall -Wextra)


target_link_libraries(
    slamd_common

    PUBLIC
        spdlog::spdlog
        slamd_flatbuff
        cxxopts
        fmt::fmt

    PRIVATE
        spdlog::spdlog
)


# WINDOW
add_executable(slamd_window)

target_sources(
    slamd_window

    PRIVATE
        src/window/main.cpp

        src/window/camera_2d.cpp
        src/window/image_texture.cpp
        src/window/glfw.cpp
        src/window/frame_buffer.cpp
        src/window/camera.cpp
        src/window/shaders.cpp
        src/window/arcball.cpp
        src/window/frame_timer.cpp
        src/window/run_window.cpp
        src/window/state_manager.cpp
        src/window/connection.cpp
        src/window/message.cpp

        src/window/tree/tree.cpp
        src/window/tree/node.cpp
        src/window/tree/tree_path.cpp

        src/window/view/view.cpp
        src/window/view/scene_view.cpp
        src/window/view/canvas_view.cpp

        src/window/geom/points_2d.cpp
        src/window/geom/geometry.cpp
        src/window/geom/deserialize.cpp
        src/window/geom/box.cpp
        src/window/geom/mesh.cpp
        src/window/geom/arrows.cpp
        src/window/geom/sphere.cpp
        src/window/geom/triad.cpp
        src/window/geom/poly_line.cpp
        src/window/geom/xy_grid.cpp
        src/window/geom/arcball_indicator.cpp
        src/window/geom/image.cpp
        src/window/geom/point_cloud.cpp
        src/window/geom/camera_frustum.cpp
        src/window/geom/poly_line_2d.cpp
        src/window/geom/mono_instanced.cpp
        src/window/geom/circles_2d.cpp
)

target_include_directories(
    slamd_window

    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${LIB_DIR}/glm>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${LIB_DIR}/asio/asio/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    slamd_window
    
    PRIVATE
        glfw
        spdlog::spdlog
        slamd_flatbuff
        slamd_common
    PUBLIC
        glbinding::glbinding
        glbinding::glbinding-aux
        imgui::imgui
)

target_compile_options(slamd_window PRIVATE -Wall -Wextra)

# SLAMD
add_library(slamd STATIC)
add_library(slamd::slamd ALIAS slamd)

# pass the path of the window executable into the lib

target_compile_definitions(
    slamd

    PRIVATE
        EXEC_PATH=\"$<TARGET_FILE:slamd_window>\"
)

target_sources(
    slamd

    PRIVATE
        src/slamd/visualizer.cpp
        src/slamd/view.cpp
        src/slamd/spawn_window.cpp

        src/slamd/net/connection.cpp
        src/slamd/net/client_set.cpp
        src/slamd/net/broadcaster.cpp

        src/slamd/tree/tree.cpp
        src/slamd/tree/tree_path.cpp

        src/slamd/geom/points_2d.cpp
        src/slamd/geom/geometry.cpp
        src/slamd/geom/box.cpp
        src/slamd/geom/mesh.cpp
        src/slamd/geom/arrows.cpp
        src/slamd/geom/sphere.cpp
        src/slamd/geom/triad.cpp
        src/slamd/geom/poly_line.cpp
        src/slamd/geom/image.cpp
        src/slamd/geom/point_cloud.cpp
        src/slamd/geom/camera_frustum.cpp
        src/slamd/geom/poly_line_2d.cpp
        src/slamd/geom/mono_instanced.cpp
        src/slamd/geom/circles_2d.cpp
)

# target_include_directories(
#     slamd

#     PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#         $<INSTALL_INTERFACE:include>
#         ${LIB_DIR}/glm
#         ${LIB_DIR}/asio/asio/include
# )

target_include_directories(
    slamd

    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${LIB_DIR}/glm>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${LIB_DIR}/asio/asio/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    slamd 
    
    PRIVATE
        spdlog::spdlog

    PUBLIC
        slamd_flatbuff
        slamd_common
)
target_compile_options(slamd PRIVATE -Wall -Wextra)




# only install if this is the top-level dir
if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
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

    install(TARGETS slamd_common

    EXPORT slamdTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        INCLUDES DESTINATION include
    )

    install(TARGETS slamd_flatbuff

    EXPORT slamdTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        INCLUDES DESTINATION include
    )
endif()
