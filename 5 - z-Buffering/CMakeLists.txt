cmake_minimum_required(VERSION 3.15)
project(CppRenderer)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(Zbuffering
        zbuffering.cpp
        model.cpp
        tgaimage.cpp)
