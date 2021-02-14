cmake_minimum_required(VERSION 3.18)

project(engine VERSION 1.1 LANGUAGES CXX C)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_library(engine
            engine.hxx
            engine.cxx)

target_compile_features(engine PUBLIC cxx_std_20)
target_link_libraries(engine INTERFACE project_compiler_flags)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(SDL2 REQUIRED)

if(MINGW)
    set(SDL2 "C:/Program Files/SDL2/lib")
    target_include_directories(engine PRIVATE "C:/Program Files/SDL2/include/SDL2")
    target_link_libraries(engine PRIVATE
        SDL2::SDL2
        SDL2
        )
    elseif(UNIX)
        target_link_libraries(engine PRIVATE
            SDL2::SDL2
            SDL2::SDL2main
            GL
            )
endif()
