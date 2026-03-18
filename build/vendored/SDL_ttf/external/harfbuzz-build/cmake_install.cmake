# Install script for directory: /home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/ucrt64/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/harfbuzz" TYPE FILE FILES
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-aat-layout.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-aat.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-blob.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-buffer.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-common.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-cplusplus.hh"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-deprecated.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-draw.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-face.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-font.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-map.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-color.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-deprecated.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-font.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-layout.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-math.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-meta.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-metrics.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-name.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-shape.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-var.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-ot.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-paint.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-set.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-shape-plan.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-shape.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-style.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-unicode.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-version.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb.h"
    "/home/studentcoll/Sand/vendored/SDL_ttf/external/harfbuzz/src/hb-ft.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/studentcoll/Sand/build/vendored/SDL_ttf/external/harfbuzz-build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
