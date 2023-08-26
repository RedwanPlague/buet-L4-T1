cmake_minimum_required(VERSION 3.19)
project(spoofing C)

set(CMAKE_C_STANDARD 11)

add_executable(server server.c)
add_executable(attacker attacker.c)
add_executable(client client.c)
