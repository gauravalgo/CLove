#
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
#

cmake_minimum_required(VERSION 2.8)

project(clove)

if (UNIX)
    add_definitions(-std=gnu99 -O3)
endif()
#if (MINGW)
# add_definitions(-std=c99 -O3)
#endif()

IF(MINGW)
FIND_PACKAGE(OpenGL REQUIRED)
SET(MTCL_BUILD_FLAGS "")
SET(OS_SPECIFIC_LIBRARIES)
ENDIF(MINGW)

IF(UNIX AND NOT APPLE)
FIND_PACKAGE(OpenGL REQUIRED)
SET(MTCL_BUILD_FLAGS "")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MTCL_BUILD_FLAGS} -fPIC -DLINUX -ldl")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D__Debug__")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MTCL_BUILD_FLAGS} -fPIC -DLINUX -ldl")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__Debug__")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
SET(CMAKE_SHARED_LINKER_FLAGS "-ldl -Wl,--as-needed,--allow-multiple-definition,--build-id")
SET(OS_SPECIFIC_LIBRARIES pthread Xxf86vm X11 Xrandr Xi)
ENDIF(UNIX AND NOT APPLE)

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
    src/physics/*.c
    src/physics/*.h
    src/*.c
    src/*.h

    #3rdparty "small" libs
    src/3rdparty/stb/*.c
    src/3rdparty/stb/*.h
    src/3rdparty/noise/*.c
    src/3rdparty/noise/*.h
    src/3rdparty/slre/*.c
    src/3rdparty/slre/*.h
    )

set(clove_package "clove")
set(clove_creator "Muresan Vlad")
set(clove_copyright "Copyright (c) 2015-2016, ${clove_creator}.")

add_executable(
    clove
    ${SOURCE_FILES}
    )
INCLUDE_DIRECTORIES(src/3rdparty/stb)

#chipmunk(physics)
add_subdirectory(src/3rdparty/chipmunk/)
set(CHIPMUNK_INCLUDE_DIR src/3rdparty/chipmunk/include/)

#freetype
add_subdirectory(src/3rdparty/freetype)
set(FREETYPE_INCLUDE_DIR src/3rdparty/freetype/include)

#openal
add_subdirectory(src/3rdparty/openal)
set(OPENAL_INCLUDE_DIR src/3rdparty/openal/include/)

#lua
add_subdirectory(src/3rdparty/lua)
set(LUA_INCLUDE_DIR src/3rdparty/lua/)
set(LUA_LIBRARIES lua)

#glew
add_subdirectory(src/3rdparty/glew/)
set(GLEW_INCLUDE_DIR src/3rdparty/glew/include/GL/)

#sdl on Windows is stupid so we'll use glfw
if(UNIX)
    add_subdirectory(src/3rdparty/SDL2/)
    include_directories(src/3rdparty/SDL2/include/)
    set(SDL_INCLUDE_DIR src/3rdparty/SDL2/include/)
    set(SDL_LIBRARIES SDL2)
endif()
if(MINGW) 
    add_subdirectory(src/3rdparty/glfw)
    include_directories(src/3rdparty/glfw/include/)
    set(GLFW_LIBRARIES "glfw")
endif()
#end sdl

add_definitions(-DAL_LIBTYPE_STATIC)
add_definitions(-DGLEW_STATIC)
add_definitions(-DFT2_BUILD_LIBRARY)

#freetype
set(FREETYPE_LIBRARIES freetype)
include_directories(src/3rdparty/freetype/include)

set(CHIPMUNK_LIBRARIES chipmunk_static)
set(GLEW_LIBRARIES libglew_static)

if(MINGW)
    SET(OPENAL_LIBRARIES OpenAL32)
endif()
if(UNIX)
    SET(OPENAL_LIBRARIES openal)
endif()

if(MINGW)
    target_link_libraries(clove ws2_32.lib glfw
        ${GLFW_LIBRARIES} ${GLEW_LIBRARIES}
        ${CHIPMUNK_LIBRARIES}
        ${OPENGL_LIBRARIES} ${OPENAL_LIBRARIES}
        ${FREETYPE_LIBRARIES} ${LUA_LIBRARIES})
endif()

if(APPLE)
    target_link_libraries(clove
        ${SDL_LIBRARIES} ${GLEW_LIBRARIES}
        ${CHIPMUNK_LIBRARIES}
        ${OPENGL_LIBRARIES} ${OPENAL_LIBRARIES}
        ${FREETYPE_LIBRARIES} ${LUA_LIBRARIES})
endif()

if(UNIX)
    target_link_libraries(clove
        ${SDL_LIBRARIES} ${GLEW_LIBRARIES}
        ${CHIPMUNK_LIBRARIES}
        ${OPENGL_LIBRARIES} ${OPENAL_LIBRARIES}
        ${FREETYPE_LIBRARIES} ${LUA_LIBRARIES})
endif()

