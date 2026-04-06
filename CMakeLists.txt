cmake_minimum_required(VERSION 3.16)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED true)

if (BUILD_TESTING)
  set (CMAKE_BUILD_TYPE "Debug")
endif()

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


add_library(include_headers INTERFACE)
target_sources(include_headers
  INTERFACE
    FILE_SET include_headers
    TYPE HEADERS
    BASE_DIRS
      main/include
    FILES
      main/include/classes.hpp
      main/include/config.hpp
)

add_executable(game WIN32
  main/main.cpp
  main/classes.cpp
)

# Link to the actual SDL3 library.
target_link_libraries(game PRIVATE include_headers SDL3::SDL3 SDL3_ttf::SDL3_ttf)

add_custom_command(
  TARGET game POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/vendored/UnifontExMono.ttf
    $<TARGET_FILE_DIR:game>/vendored/UnifontExMono.ttf
)

# For testing
if (BUILD_TESTING)
  # https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#top
  add_subdirectory(vendored/catch2 EXCLUDE_FROM_ALL)

  add_executable(game_test
    test/test.cpp
  )

  target_link_libraries(game_test PRIVATE include_headers SDL3::SDL3 SDL3_ttf::SDL3_ttf
                        Catch2::Catch2WithMain)

  include(CTest)
  include(Catch)
  catch_discover_tests(game_test)
endif()
