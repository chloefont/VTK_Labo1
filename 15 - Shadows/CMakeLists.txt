cmake_minimum_required(VERSION 3.15)
project(CppRenderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-gnu-anonymous-struct -Wno-nested-anon-types")

include_directories(.)

set(COMMON_CPP_FILES
        model.cpp
        Camera.cpp
        Viewport.cpp
        Triangle.cpp
        tgaimage.cpp
        Shader.cpp
        NormalTextureShader.cpp
        GouraudTextureShader.cpp
        FlatTextureShader.cpp
        FlatShader.cpp
        GouraudShader.cpp
        NormalShader.cpp
        PhongShader.cpp
        PhongTextureShader.cpp
        PhongNormalShader.cpp
        PhongNormalTextureShader.cpp
        TGAColor.cpp
        ShadowShader.cpp)

#add_executable(TestAllShaders
#        TestAllShaders.cpp
#        ${COMMON_CPP_FILES}
#        )

add_executable(OneShader
        OneShader.cpp
        ${COMMON_CPP_FILES}
        )
