cmake_minimum_required(VERSION 3.18)

project(test_game LANGUAGES CXX)

add_executable(test_game game.cxx)

target_compile_features(test_game PUBLIC cxx_std_20)
target_include_directories(test_game PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/engine)
target_link_libraries(test_game PRIVATE
                                        engine
#                                INTERFACE
#                                        project_compiler_flags
                                        )
