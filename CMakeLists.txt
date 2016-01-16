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
if(MINGW)
    add_definitions(-std=c99 -O3)
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
#add_definitions(-DFT2_BUILD_LIBRARY)

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