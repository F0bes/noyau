cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
project(noyau C CXX ASM)

add_subdirectory(loader)
add_subdirectory(kernel)
