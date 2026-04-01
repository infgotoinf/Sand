cmake_minimum_required(VERSION 3.16)
project(Sand)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED true)
# set (CMAKE_BUILD_TYPE Debug)

project(Sand)

# This chunk of code exists to make my clangd LSP in Helix work with SDL3
# https://mzhang.io/posts/2022-03-03-clangd-in-nix/
# Generate the `compile_commands.json` file.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
    ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

# Use vendored libs
set(SDLTTF_VENDORED ON)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(game WIN32)

target_sources(game
PRIVATE
  main/main.cpp
  main/classes.cpp

PUBLIC
  FILE_SET include_headers
  TYPE HEADERS
  BASE_DIRS
    main/include
  FILES
    main/include/classes.hpp
)

# Link to the actual SDL3 library.
target_link_libraries(game PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)
