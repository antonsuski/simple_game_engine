cmake_minimum_required(VERSION 3.18)

project(test_game LANGUAGES CXX)

add_executable(test_game game.cxx)

target_link_libraries(test_game PRIVATE engine
                                INTERFACE project_compiler_flags)
