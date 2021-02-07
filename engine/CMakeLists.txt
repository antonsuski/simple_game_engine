cmake_minimum_required(VERSION 3.18)

project(engine VERSION 1.1 LANGUAGES CXX C)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_library(engine
            engine.hxx
            engine.cxx)
target_compile_features(engine PUBLIC cxx_std_20)
target_link_libraries(engine INTERFACE project_compiler_flags)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
