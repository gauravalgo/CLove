cmake_minimum_required(VERSION 2.8)

project(love)

add_definitions(-std=c11 -O3)

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
set(FREETYPE_LIBRARIES freetype)
include_directories(src/3rdparty/freetype/include)

#openal
add_subdirectory(src/3rdparty/openal)
set(OPENAL_INCLUDE_DIR src/3rdparty/openal/include/)
set(OPENAL_LIBRARIES "${OPENAL_LIBRARY}")

#lua
add_subdirectory(src/3rdparty/lua)
set(LUA_INCLUDE_DIR src/3rdparty/lua/ ${CMAKE_BINARY_DIR}/src/3rdparty/lua/)
SET(LUA_LIBRARIES lua)

#glew
add_subdirectory(src/3rdparty/glew/)
SET(GLEW_INCLUDE_DIR src/3rdparty/glew/include/GL/)

#sdl
ADD_SUBDIRECTORY(src/3rdparty/sdl2/)
SET(SDL_INCLUDE_DIR src/3rdparty/sdl2/include/)

SET(FREETYPE_LIBRARIES freetype)
SET(SDL_LIBRARIES SDL2)
SET(GLEW_LIBRARIES libglew_static)
SET(OPENAL_LIBRARY OpenAL32)

if(WIN32)
    target_link_libraries(love ws2_32.lib
        ${SDL_LIBRARIES} ${GLEW_LIBRARIES}
        ${OPENGL_LIBRARIES} ${OPENAL_LIBRARIES}
        ${FREETYPE_LIBRARIES} ${LUA_LIBRARIES})
endif()

if(APPLE)
    target_link_libraries(love sdl ${SDL_LIBRARIES}
        ${GLEW_LIBRARIES} ${OPENAL_LIBRARIES} ${FREETYPE_LIBRARIES}
        ${LUA_LIBRARIES})
endif()

if(UNIX)
    target_link_libraries(love dl ${SDL_LIBRARIES}
        ${GLEW_LIBRARIES} ${OPENAL_LIBRARIES} ${FREETYPE_LIBRARIES}
        ${LUA_LIBRARIES})
endif()
