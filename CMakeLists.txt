cmake_minimum_required(VERSION 3.15)
include_guard(GLOBAL)
project(server)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 3rd part subdirectories go first
include(FetchuWebSockets.cmake)
add_subdirectory(../3rd_party/pybind11 pybind11)
add_subdirectory(../3rd_party/json nlohmann_json)

# Own libraries using 3rd parties come next
add_subdirectory(light_control)

add_executable(server
    main.cpp
)


target_link_libraries(server
    uWebSockets
    ${PYTHON_LIBRARIES}
    pybind11::headers
    nlohmann_json::nlohmann_json
    nicenice::light_control
)

# Make CMake generate a header to fill in the current source path, then let files include it
configure_file( file_paths.h.in ${CMAKE_BINARY_DIR}/generated/file_paths.h )
target_include_directories(server
PRIVATE
    ${CMAKE_BINARY_DIR}/generated
)