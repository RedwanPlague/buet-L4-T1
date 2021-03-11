cmake_minimum_required(VERSION 3.17)
project(offline_1_code)

set(CMAKE_CXX_STANDARD 14)
set(OpenGlLinkers -lglut -lGLU -lGL) # setting all the Glut libraries as one variable

add_executable(test test.cpp)
target_link_libraries(test ${OpenGlLinkers}) # linking opengl libraries to the project

add_executable(problem-1 problem-1.cpp)
target_link_libraries(problem-1 ${OpenGlLinkers}) # linking opengl libraries to the project

add_executable(problem-2 problem-2.cpp)
target_link_libraries(problem-2 ${OpenGlLinkers}) # linking opengl libraries to the project