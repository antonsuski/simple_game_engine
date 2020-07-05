cmake_minimum_required(VERSION 3.17)

project(engine LANGUAGES CXX C)

add_library(engine_shared SHARED
    src/engine.cxx
    src/shader.cxx
    src/texture2d.cxx
    src/stuff.cxx
    src/stb_image.cxx
    src/vbo.cxx
    include/engine.hxx
    include/glad/glad.c
    include/glad/glad.h
    include/shader.hxx
    include/om_gl_check.hxx
    include/picopng.hxx
    include/stb_image.h
    include/texture2d.hxx
    include/stuff.hxx
    include/vbo.hxx)

find_package(SDL2 REQUIRED)

target_compile_features(engine_shared PUBLIC cxx_std_20)
target_include_directories(engine_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(engine_shared PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    GL)
