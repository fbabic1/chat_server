cmake_minimum_required(VERSION "3.28.3")

project(chat_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(lib)

add_executable(client client.cpp)
target_link_libraries(client PRIVATE lib)

add_executable(server server.cpp)
target_link_libraries(server PRIVATE lib)