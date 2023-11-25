cmake_minimum_required(VERSION 3.25)
project(learn_opengl)

set(CMAKE_CXX_STANDARD 17)

add_executable(learn_opengl main.cpp GLCamera.h GLCamera.h)

add_subdirectory(chapter2-4)
add_subdirectory(chapter2-5)
add_subdirectory(chapter2-6)
add_subdirectory(chapter2-7)
add_subdirectory(chapter2-8)
add_subdirectory(chapter2-9)
add_subdirectory(chapter2-10)
add_subdirectory(chapter2-11)
add_subdirectory(chapter3-1)