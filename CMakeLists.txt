  cmake_minimum_required(VERSION 2.8)

  project(love C)

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

  
  include_directories(src/3rdparty/stb)
  include_directories(src/3rdparty/glew/include)

   add_executable(
    love
    ${SOURCE_FILES}
    src/3rdparty/glew/src/glew.c
    src/3rdparty/stb/stb_image.c
    src/3rdparty/stb/stb_vorbis.c
    )

add_definitions(-std=c99 -O3)

#freetype
add_subdirectory(src/3rdparty/freetype)
set(FREETYPE_INCLUDE_DIR src/3rdparty/freetype/include)
set(FREETYPE_LIBRARIES freetype)
include_directories(src/3rdparty/freetype/include)

#openal
add_subdirectory(src/3rdparty/openal)
set(OPENAL_INCLUDE_DIR src/3rdparty/openal/include)
set(OPENAL_LIBRARIES "${OPENAL_LIBRARY}")

#lua
add_subdirectory(src/3rdparty/lua)
set(LUA_INCLUDE_DIR src/3rdparty/lua/ ${CMAKE_BINARY_DIR}/src/3rdparty/lua/)
SET(LUA_LIBRARIES lua)

  IF(MINGW)
  FIND_PACKAGE(OpenGL REQUIRED)

  LINK_DIRECTORIES("$ENV{DXSDK_DIR}/Lib/$ENV{PROCESSOR_ARCHITECTURE}")
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # using GCC
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D__Debug__ -I\"$ENV{DXSDK_DIR}/Include\"")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__Debug__ -I\"$ENV{DXSDK_DIR}/Include\"")

    SET(CMAKE_SHARED_LINKER_FLAGS "-lcomctl32 -lkernel32 -Wl,--subsystem,windows,--as-needed,--allow-multiple-definition -static -static-libstdc++ -static-libgcc")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--allow-multiple-definition -static -static-libstdc++ -static-libgcc")
    SET(CPACK_STRIP_FILES ON)

  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # using Visual Studio C++

    IF(SSE2_TRUE)
    SET(SSE_FLAG /arch:SSE2)
  ELSEIF(SSE_TRUE)
    SET(SSE_FLAG /arch:SSE)
    ENDIF()

    SET(flag_var "/MD")
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Z7 /W4 /Od /MDd /D_DEBUG /D__Debug__")
    SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /W1 /O2 /Ot /MD /DNDEBUG /fp:fast ${SSE_FLAG}")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Z7 /W4 /Od /MDd /D_DEBUG /D__Debug__")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W1 /O2 /Ot /MD /DNDEBUG /fp:fast ${SSE_FLAG}")
  endif()

  SET(FREETYPE_LIBRARY "freetype")
  SET(OPENAL_LIBRARY "OpenAL32")
ENDIF(MINGW)

  ## Dependences 
  SET(GLEW_INCLUDE_DIR src/3rdparty/glew/include)
  SET(SDL_INCLUDE_DIR src/3rdparty/SDL2/include)

  SET(FREETYPE_LIBRARIES freetype)
  SET(GLEW_LIBRARIES GLEW_static)
  SET(SDL_LIBRARIES SDL2main SDL2-static)
  SET(OPENAL_LIBRARY "OpenAL32")

if(MINGW)
  target_link_libraries(love ws2_32.lib 
    openal ${OPENAL_LIBRARIES} ${FREETYPE_LIBRARIES} ${LUA_LIBRARIES})
endif()
if(APPLE)
  target_link_libraries(love 
    openal ${OPENAL_LIBRARIES} ${FREETYPE_LIBRARIES} ${LUA_LIBRARIES})
endif()

if(UNIX)
  target_link_libraries(love dl 
    openal ${OPENAL_LIBRARIES} ${FREETYPE_LIBRARIES} ${LUA_LIBRARIES})
endif()