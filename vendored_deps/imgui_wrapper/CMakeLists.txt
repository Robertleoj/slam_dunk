cmake_minimum_required(VERSION 3.14)
project(imgui)

file(GLOB IMGUI_SRC CONFIGURE_DEPENDS
    imgui/*.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})
add_library(imgui::imgui ALIAS imgui)

target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

target_link_libraries(imgui PUBLIC glfw)
