cmake_minimum_required(VERSION 3.16)

project(engine VERSION 1.1 LANGUAGES CXX C)

# option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_library(engine
            engine.hxx
            gl_assist.hxx
            shader.hxx
            stuff.hxx
            vbo.hxx
            texture2d.hxx
            stb_image.h
            glad.h

            glad.c
            shader.cxx
            stuff.cxx
            vbo.cxx
            texture2d.cxx
            stb_image.cxx
            engine.cxx)

target_compile_features(engine PUBLIC cxx_std_20)
target_link_libraries(engine INTERFACE project_compiler_flags)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
    set(glm_DIR "F:/libs/glm-master/glm-master/cmake/glm")
endif()

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)

# set path file for linking with debug SDL2
#set(SDL2_LIB_D "/home/antonsyski/devel/repos/simple_game_engine/build/SDL2-2.0.14/libSDL2-2.0d.so.0.14.0")

target_include_directories(engine PRIVATE ${SDL2_INCLUDE_DIRS})

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
        glm::glm

        #${SDL2_LIB_D}
        #GL
        )
endif()
