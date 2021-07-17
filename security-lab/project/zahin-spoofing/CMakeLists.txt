cmake_minimum_required(VERSION 3.19)
project(zahin_spoofing)

set(CMAKE_CXX_STANDARD 11)

add_executable(client client.cpp)
add_executable(server server.cpp)
