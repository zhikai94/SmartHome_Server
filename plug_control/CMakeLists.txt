cmake_minimum_required(VERSION 3.15)
include_guard(GLOBAL)
project(plug_control)

add_library(plug_control
    plug_control.cpp
)
add_library(nicenice::plug_control ALIAS plug_control)

target_link_libraries(plug_control
PUBLIC
    ${PYTHON_LIBRARIES}
    pybind11::headers
    nlohmann_json::nlohmann_json
)

target_include_directories(plug_control
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
