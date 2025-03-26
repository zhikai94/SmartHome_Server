cmake_minimum_required(VERSION 3.15)
include_guard(GLOBAL)
project(light_control)

add_library(light_control
    light_control.cpp
)
add_library(nicenice::light_control ALIAS light_control)

target_link_libraries(light_control
PUBLIC
    ${PYTHON_LIBRARIES}
    pybind11::headers
    nlohmann_json::nlohmann_json
)

target_include_directories(light_control
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
