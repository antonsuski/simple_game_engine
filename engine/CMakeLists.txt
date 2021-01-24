cmake_minimum_required(VERSION 3.17)

project(engine LANGUAGES CXX C)



add_library(engine_shared SHARED
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
if(MINGW)
        find_package(SDL2 CONFIG REQUIRED)
else()
	find_package(SDL2 CONFIG REQUIRED)
endif()

find_package(SDL2 REQUIRED)
#find_library(GLM REQUIRED)


target_compile_features(engine_shared PUBLIC cxx_std_17)
target_include_directories(engine_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(engine_shared PUBLIC imgui)
target_link_libraries(engine_shared PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    GL)
