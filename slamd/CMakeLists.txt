cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SLAMD_ENABLE_INSTALL "Enable install targets" OFF)

project(slam_dunk VERSION 0.1.0)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(LIB_DIR vendored_deps)
set(BIN_DIR ${CMAKE_BINARY_DIR}/_deps)

function(find_required_package 
    pkg_name 
    target_name 
    found_var 
    subdir
)
    if (NOT TARGET ${target_name})
        find_package(${pkg_name} CONFIG QUIET)
        if (NOT ${found_var})
            message(STATUS "${pkg_name} not found via find_package, trying add_subdirectory, bro.")
            add_subdirectory(
                ${subdir} 
                # ${CMAKE_BINARY_DIR}/${pkg_name}
            )
        endif()
        # After add_subdirectory, check again
        if (NOT TARGET ${target_name})
            message(FATAL_ERROR "${pkg_name} not found, even after add_subdirectory, bro! We're toast.")
        endif()
    endif()
endfunction()

# Slam those deps in with confidence:
find_required_package(
    glbinding 
    glbinding::glbinding 
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

add_subdirectory(${LIB_DIR}/glm ${BIN_DIR}/glm)


add_library(slamd STATIC)
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

        src/geom/utils.cpp
        src/geom/points_2d.cpp
        src/geom/geometry.cpp
        src/geom/box.cpp
        src/geom/simple_mesh.cpp
        src/geom/arrows.cpp
        src/geom/sphere.cpp
        src/geom/triad.cpp
        src/geom/mono_mesh.cpp
        src/geom/poly_line.cpp
        src/geom/xy_grid.cpp
        src/geom/arcball_indicator.cpp
        src/geom/image.cpp
        src/geom/point_cloud.cpp
        src/geom/camera_frustum.cpp
)

target_include_directories(
    slamd

    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${LIB_DIR}/glm
)

target_link_libraries(
    slamd 
    
    PRIVATE
        glfw
        spdlog::spdlog
        imgui::imgui

    PUBLIC
        glbinding::glbinding
        glbinding::glbinding-aux
)


if(SLAMD_ENABLE_INSTALL)
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
endif()
