cmake_minimum_required(VERSION 3.19)
project(dhcp C)

set(CMAKE_C_STANDARD 11)

add_executable(main main.c)
add_executable(raw raw.c)
