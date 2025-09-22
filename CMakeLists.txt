cmake_minimum_required(VERSION "3.28.3")

project(client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(${PROJECT_NAME} client.cpp)

# -------------------------
project(server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(${PROJECT_NAME} server.cpp)