cmake_minimum_required(VERSION 3.14)
project(myviz_bindings LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(LIB_DIR vendored_deps)


add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_OFF)


if(NOT TARGET spdlog::spdlog)
    message(STATUS "Adding spdlog...")
    add_subdirectory(${LIB_DIR}/spdlog)
endif()

if(NOT TARGET glbinding::glbinding-aux)
    message(STATUS "Adding glbinding...")
    add_subdirectory(${LIB_DIR}/glbinding)
    install(TARGETS glbinding DESTINATION slamd OPTIONAL)
    install(TARGETS glbinding-aux DESTINATION slamd OPTIONAL)
endif()

if(NOT TARGET glfw)
    message(STATUS "Adding glfw...")
    add_subdirectory(${LIB_DIR}/glfw)
endif()

if(NOT TARGET imgui::imgui)
    message(STATUS "Adding imgui...")
    add_subdirectory(${LIB_DIR}/imgui_wrapper)
endif()


if(NOT TARGET slamd)
    message(STATUS "Adding slamd...")
    add_subdirectory(slamd_src)
endif()


find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)

# Try to find installed pybind11
find_package(pybind11 CONFIG QUIET)

if(NOT pybind11_FOUND)
    message(STATUS "pybind11 not found, using FetchContent to grab it")
    include(FetchContent)
    FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG        stable
    )
    FetchContent_MakeAvailable(pybind11)
endif()

# === pybind11 module ===
pybind11_add_module(bindings src/main.cpp)


target_include_directories(
    bindings

    PRIVATE

        ${LIB_DIR}/glm
        ${LIB_DIR}/glbinding/include
        ${LIB_DIR}/glfw/include
)

target_link_libraries(bindings 
    PRIVATE 
        slamd::slamd
        imgui::imgui
        glfw
        glbinding::glbinding
        glbinding::glbinding-aux
)

set_target_properties(bindings PROPERTIES
    INSTALL_RPATH "$ORIGIN"
)

# === Install the pybind11 module ===
install(TARGETS bindings DESTINATION slamd)

# === Install your shared libraries ===

# Example: install slamd if it's a shared lib
