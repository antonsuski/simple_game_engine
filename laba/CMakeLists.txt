cmake_minimum_required(VERSION 3.17)

project(laba LANGUAGES CXX)

add_executable(laba lab1_cosi.cxx)

target_compile_features(laba PUBLIC cxx_std_20)
target_link_libraries(laba PRIVATE
    engine
    )
