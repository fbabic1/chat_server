cmake_minimum_required(VERSION 3.28)

add_library(lib INTERFACE)
add_subdirectory(config)
target_link_libraries(lib INTERFACE lib_config)

add_subdirectory(utils)
target_link_libraries(lib INTERFACE lib_utils)

target_include_directories(lib INTERFACE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)