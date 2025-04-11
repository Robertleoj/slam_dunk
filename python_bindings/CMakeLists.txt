cmake_minimum_required(VERSION 3.14)
project(myviz_bindings LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(LIB_DIR ../vendored_deps)
set(LIB_BIN_DIR ${CMAKE_BINARY_DIR}/_deps)


add_subdirectory(${LIB_DIR}/spdlog ${LIB_BIN_DIR}/spdlog)
add_subdirectory(${LIB_DIR}/glbinding ${LIB_BIN_DIR}/glbinding)

add_subdirectory(${LIB_DIR}/glfw ${LIB_BIN_DIR}/glfw)
add_subdirectory(${LIB_DIR}/imgui ${LIB_BIN_DIR}/imgui)
add_library(imgui::imgui ALIAS imgui)

add_subdirectory(${LIB_DIR}/glm ${LIB_BIN_DIR}/glm)
add_subdirectory(../slamd ${CMAKE_BINARY_DIR}/slamd)

find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
find_package(pybind11 CONFIG REQUIRED)

# === pybind11 module ===
pybind11_add_module(_core src/main.cpp)


target_include_directories(
    _core

    PRIVATE


    ${LIB_DIR}/glm
    ${LIB_DIR}/glbinding/include
    ${LIB_DIR}/glfw/include
)

target_link_libraries(_core PRIVATE

    slamd::slamd
    glbinding::glbinding
    glbinding::glbinding-aux

    spdlog::spdlog

    glfw

    imgui::imgui
    glm::glm
)

set_target_properties(_core PROPERTIES
    INSTALL_RPATH "$ORIGIN"
)

# === Install the pybind11 module ===
install(TARGETS _core DESTINATION slamd)

# === Install your shared libraries ===

# Example: install slamd if it's a shared lib
install(TARGETS glbinding DESTINATION slamd OPTIONAL)
install(TARGETS glbinding-aux DESTINATION slamd OPTIONAL)
