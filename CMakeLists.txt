    cmake_minimum_required(VERSION 2.8)

    project(love)
   
    if(WIN32)
      SET(CMAKE_C_COMPILER "C:/MinGW/bin/gcc.exe")
      SET(CMAKE_CXX_COMPILER "C:/MinGW/bin/g++.exe")
      SET(CMAKE_CXX_FLAGS "-static-libgcc -static-libstdc++ -static -O3 -s")
      SET(CMAKE_C_FLAGS "-static-libgcc -static-libstdc++ -static -O3 -s")
      SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "-static-libgcc -static-libstdc++ -static -O3 -s")
      SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-static-libgcc -static-libstdc++ -static -O3 -s")
    endif()

    add_definitions(-std=c99 -O3)
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
      src/3rdparty/glew/src/glew.c
      src/3rdparty/stb/stb_image.c
      src/3rdparty/stb/stb_vorbis.c
      )
    
    include_directories(src/3rdparty/stb)
    include_directories(src/3rdparty/glew/include)

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

  SET(GLEW_INCLUDE_DIR src/3rdparty/glew/include)
  add_subdirectory(src/3rdparty/SDL2/)
  INCLUDE_DIRECTORIES(src/3rdparty/SDL2/include)
  SET(SDL_INCLUDE_DIR src/3rdparty/SDL2/include)

    SET(FREETYPE_LIBRARIES freetype)
    SET(GLEW_LIBRARIES GLEW_static)
    SET(SDL_LIBRARIES SDL2main SDL2-static)
    SET(OPENAL_LIBRARY "OpenAL32")

  if(MINGW)
    target_link_libraries(love ws2_32.lib sdl ${SDL_LIBRARIES}
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