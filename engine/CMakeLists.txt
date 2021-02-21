cmake_minimum_required(VERSION 3.18)

project(engine VERSION 1.1 LANGUAGES CXX C)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_library(engine
            engine.hxx
            gl_assist.hxx
            shader.hxx
            stuff.hxx
            vbo.hxx
            texture2d.hxx
            stb_image.h
            glad/glad.h

            glad/glad.c
            shader.cxx
            stuff.cxx
            vbo.hxx
            texture2d.cxx
            stb_image.cxx
            engine.cxx)

target_compile_features(engine PUBLIC cxx_std_20)
target_link_libraries(engine INTERFACE project_compiler_flags)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(glm_DIR "F:/libs/glm-master/glm-master/cmake/glm")

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)

if(MINGW)
    target_link_libraries(engine PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
        glm::glm
        )
elseif(UNIX)
    target_link_libraries(engine PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
        GL
        )
endif()
