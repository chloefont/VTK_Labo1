cmake_minimum_required(VERSION 3.15)
project(CppRenderer)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(RandomHead
        random_head.cpp
        model.cpp
        tgaimage.cpp)

add_executable(FlatShading flat_shading.cpp model.cpp tgaimage.cpp)
