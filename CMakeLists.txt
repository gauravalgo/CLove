#
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
#

cmake_minimum_required(VERSION 2.8)

project(love)

if (UNIX)
    add_definitions(-std=gnu99 -O3)
endif()
if (MINGW)
    add_definitions(-std=c99 -O3)
endif()

if (APPLE)
  
  add_definitions(-std=gnu99 -O3)
  SET(MACOSX_RPATH 1)
  # use, i.e. don't skip the full RPATH for the build tree
  SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

  # when building, don't use the install RPATH already
  # (but later on when installing)
  SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

  SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

  # add the automatically determined parts of the RPATH
  # which point to directories outside the build tree to the install RPATH
  SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


  # the RPATH to be used when installing, but only if it's not a system directory
  LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
  IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  ENDIF("${isSystemDir}" STREQUAL "-1")

  FIND_PACKAGE(OpenGL REQUIRED)
  FIND_LIBRARY(COCOA_LIB Cocoa)
  FIND_LIBRARY(CORE_SERVICES_LIB CoreServices)
  FIND_LIBRARY(FOUNDATION_LIB Foundation)
  FIND_LIBRARY(IOKIT_LIB IOKit)
  SET(MTCL_BUILD_FLAGS "-mmacosx-version-min=10.6")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MTCL_BUILD_FLAGS} -O3 -DOSX")
  SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D__Debug__")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MTCL_BUILD_FLAGS} -O3 -DOSX")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__Debug__")
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl")
  SET(OS_SPECIFIC_LIBRARIES ${COCOA_LIB} ${CORE_SERVICES_LIB} ${FOUNDATION_LIB} ${IOKIT_LIB})
endif()

FILE(
    GLOB SOURCE_FILES
    src/audio/*.h
    src/audio/*.c
    src/filesystem/*.h
    src/filesystem/*.c
    src/graphics/*.h
    src/graphics/*.c
    src/image/*.h
    src/image/*.c
    src/luaapi/*.h
    src/luaapi/*.c
    src/tools/*.c
    src/tools/*.h
    src/math/*.h
    src/math/*.c
    src/timer/*.h
    src/timer/*.c
    src/*.c
    src/*.h
    )

set(love_package "love")
set(love_creator "Muresan Vlad")
set(love_copyright "Copyright (c) 2015-2016, ${love_creator}.")

add_executable(
    love
    ${SOURCE_FILES}
    src/3rdparty/stb/stb_image.c
    src/3rdparty/stb/stb_vorbis.c
    src/3rdparty/stb/stb_vorbis.h
    src/3rdparty/noise/simplexnoise.c
    src/3rdparty/noise/simplexnoise.h
    src/3rdparty/slre/slre.c
    src/3rdparty/slre/slre.h
    )
INCLUDE_DIRECTORIES(src/3rdparty/stb)

#freetype
add_subdirectory(src/3rdparty/freetype)
set(FREETYPE_INCLUDE_DIR src/3rdparty/freetype/include)

#openal
add_subdirectory(src/3rdparty/openal)
set(OPENAL_INCLUDE_DIR src/3rdparty/openal/include/)

#lua
add_subdirectory(src/3rdparty/lua)
set(LUA_INCLUDE_DIR src/3rdparty/lua/)
SET(LUA_LIBRARIES lua)

#glew
add_subdirectory(src/3rdparty/glew/)
SET(GLEW_INCLUDE_DIR src/3rdparty/glew/include/GL/)

#sdl
ADD_SUBDIRECTORY(src/3rdparty/SDL2/)
include_directories(src/3rdparty/SDL2/include/)
SET(SDL_INCLUDE_DIR src/3rdparty/SDL2/include/)

add_definitions(-DAL_LIBTYPE_STATIC)
add_definitions(-DGLEW_STATIC)
add_definitions(-DFT2_BUILD_LIBRARY)

SET(FREETYPE_LIBRARIES freetype)
include_directories(src/3rdparty/freetype/include)
SET(SDL_LIBRARIES SDL2)
SET(GLEW_LIBRARIES libglew_static)

if(MINGW)
SET(OPENAL_LIBRARIES OpenAL32)
endif()
if(UNIX)
SET(OPENAL_LIBRARIES openal)
endif()
if(MINGW)
    target_link_libraries(love ws2_32.lib
        ${SDL_LIBRARIES} ${GLEW_LIBRARIES}
        ${OPENGL_LIBRARIES} ${OPENAL_LIBRARIES}
        ${FREETYPE_LIBRARIES} ${LUA_LIBRARIES})
endif()

if(APPLE)
    target_link_libraries(love
    	 ${SDL_LIBRARIES} ${GLEW_LIBRARIES}
        ${OPENGL_LIBRARIES} ${OPENAL_LIBRARIES}
        ${FREETYPE_LIBRARIES} ${LUA_LIBRARIES})
endif()

if(UNIX)
    target_link_libraries(love
    	 ${SDL_LIBRARIES} ${GLEW_LIBRARIES}
        ${OPENGL_LIBRARIES} ${OPENAL_LIBRARIES}
        ${FREETYPE_LIBRARIES} ${LUA_LIBRARIES})
endif()
