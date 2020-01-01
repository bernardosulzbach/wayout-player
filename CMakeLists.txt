cmake_minimum_required(VERSION 3.11)

project(wayout-player)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  add_definitions(-Wall)
  add_definitions(-Wextra)
  add_definitions(-Wshadow)
endif()

add_definitions(-Werror)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  add_definitions(-DDEBUGGING)
  set(
    CMAKE_CXX_FLAGS

    "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fsanitize=leak"
    )
  set(
    CMAKE_EXE_LINKER_FLAGS_DEBUG

    "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fsanitize=leak"
    )
endif()

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIR})
set(Boost_USE_MULTITHREADED ON)

add_library(wayout-player OBJECT
            src/ArgumentParser.cpp
            src/ArgumentParser.hpp
            src/Board.cpp
            src/Board.hpp
            src/Types.hpp
            src/Position.cpp
            src/Position.hpp
            src/Tile.cpp
            src/Tile.hpp
            src/TileType.cpp
            src/TileType.hpp
            src/Filesystem.cpp
            src/Filesystem.hpp
            src/Solution.cpp
            src/Solution.hpp)

find_package(Threads REQUIRED)

add_executable(test test/Tests.cpp $<TARGET_OBJECTS:wayout-player>)

target_link_libraries(test ${Boost_LIBRARIES})
target_link_libraries(test ${CMAKE_THREAD_LIBS_INIT})

add_executable(player src/Player.cpp $<TARGET_OBJECTS:wayout-player>)
target_link_libraries(player ${Boost_LIBRARIES})
target_link_libraries(player ${CMAKE_THREAD_LIBS_INIT})
