cmake_minimum_required(VERSION 3.17)

project(engine LANGUAGES CXX C)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_library(engine
    src/engine.cxx
    src/shader.cxx
    src/texture2d.cxx
    src/stuff.cxx
    src/stb_image.cxx
    src/vbo.cxx
    src/input_handler.cxx
    src/imgui_impl_opengl3.cpp
    src/imgui_impl_sdl.cpp
    
    include/engine.hxx
    include/glad/glad.c
    include/glad/glad.h
    include/shader.hxx
    include/om_gl_check.hxx
    include/picopng.hxx
    include/stb_image.h
    include/texture2d.hxx
    include/stuff.hxx
    include/vbo.hxx
    include/input_handler.hxx
    include/imgui_impl_opengl3.h
    include/imgui_impl_sdl.h
)

find_package(SDL2 CONFIG REQUIRED)

set(SDL2 "C:/Program Files/SDL2/lib")

target_compile_features(engine PUBLIC cxx_std_20)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(engine PUBLIC imgui)
target_link_libraries(engine PRIVATE
    SDL2
#    SDL2::SDL2
#    SDL2::SDL2main
    GL
    )
