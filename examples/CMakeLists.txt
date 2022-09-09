cmake_minimum_required(VERSION 3.16)

project(test_game LANGUAGES CXX)

add_executable(test_game game.cxx
                         #game_object.cxx
                         #game_object.hxx
                         )
target_compile_features(test_game PUBLIC cxx_std_20)
target_include_directories(test_game PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/engine)
target_link_libraries(test_game PRIVATE
                                        engine
#                                        INTERFACE
#                                        project_compiler_flags
                                        )

add_executable(opengl_2-6_texture opengl_2-6_texture.cxx)
target_compile_features(opengl_2-6_texture PUBLIC cxx_std_20)
target_include_directories(opengl_2-6_texture PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/engine)
target_link_libraries(opengl_2-6_texture PRIVATE
                                        engine
#                                        INTERFACE
#                                        project_compiler_flags
                                        )

add_executable(opengl_2-7_transform opengl_2-7_transform.cxx)
target_compile_features(opengl_2-7_transform PUBLIC cxx_std_20)
target_include_directories(opengl_2-7_transform PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/engine)
target_link_libraries(opengl_2-7_transform   PRIVATE
                                        engine
#                                        INTERFACE
#                                        project_compiler_flags
                                        )

add_executable(abba abba.cxx)
target_compile_features(abba PUBLIC cxx_std_20)
target_include_directories(abba PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/engine)
target_link_libraries(abba   PRIVATE
                                        engine
#                                        INTERFACE
#                                        project_compiler_flags
                                        )
find_package(SDL2 REQUIRED)
add_executable(pattern_command pattern_command.cxx
                                pattern_command.hxx)
target_compile_features(pattern_command PUBLIC cxx_std_20)
target_include_directories(pattern_command PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/engine)
target_link_libraries(pattern_command   PUBLIC
                                        engine
                                        SDL2::SDL2
                                        SDL2::SDL2main                                        
#                                        INTERFACE
#                                        project_compiler_flags
)