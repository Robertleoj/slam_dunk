cmake_minimum_required(VERSION 3.14)
project(myviz_bindings LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(LIB_DIR ../vendored_deps)
set(LIB_BIN_DIR ${CMAKE_BINARY_DIR}/_deps)


if(NOT TARGET spdlog::spdlog)
    message(STATUS "Adding spdlog...")
    add_subdirectory(${LIB_DIR}/spdlog ${LIB_BIN_DIR}/spdlog)
endif()

if(NOT TARGET glbinding::glbinding)
    message(STATUS "Adding glbinding...")
    add_subdirectory(${LIB_DIR}/glbinding ${LIB_BIN_DIR}/glbinding)
    install(TARGETS glbinding DESTINATION slamd OPTIONAL)
    install(TARGETS glbinding-aux DESTINATION slamd OPTIONAL)
endif()

if(NOT TARGET glfw)
    message(STATUS "Adding glfw...")
    add_subdirectory(${LIB_DIR}/glfw ${LIB_BIN_DIR}/glfw)
endif()

if(NOT TARGET imgui::imgui)
    message(STATUS "Adding imgui...")
    add_subdirectory(${LIB_DIR}/imgui_wrapper ${LIB_BIN_DIR}/imgui)
    add_library(imgui::imgui ALIAS imgui)
endif()

if(NOT TARGET glm::glm)
    message(STATUS "Adding glm...")
    add_subdirectory(${LIB_DIR}/glm ${LIB_BIN_DIR}/glm)
endif()

if(NOT TARGET slamd)
    message(STATUS "Adding slamd...")
    add_subdirectory(../slamd ${CMAKE_BINARY_DIR}/slamd)
endif()



find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
find_package(pybind11 CONFIG REQUIRED)

# === pybind11 module ===
pybind11_add_module(bindings src/main.cpp)


target_include_directories(
    bindings

    PRIVATE

    ${LIB_DIR}/glm
    ${LIB_DIR}/glbinding/include
    ${LIB_DIR}/glfw/include
)

target_link_libraries(bindings PRIVATE

    slamd::slamd
    glbinding::glbinding
    glbinding::glbinding-aux

    spdlog::spdlog

    glfw

    imgui::imgui
    glm::glm
)

set_target_properties(bindings PROPERTIES
    INSTALL_RPATH "$ORIGIN"
)

# === Install the pybind11 module ===
install(TARGETS bindings DESTINATION slamd)

# === Install your shared libraries ===

# Example: install slamd if it's a shared lib
