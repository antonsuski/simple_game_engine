cmake_minimum_required(VERSION 3.16)

project(simple_game_engine LANGUAGES CXX C)

add_subdirectory(engine)
add_subdirectory(examples)


add_library(project_compiler_flags INTERFACE)
target_compile_features(project_compiler_flags INTERFACE cxx_std_20)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(project_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-g>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)
