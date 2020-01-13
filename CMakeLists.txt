cmake_minimum_required(VERSION 3.12)

project(wayout-player)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  add_definitions(-Wall)
  add_definitions(-Wextra)
  add_definitions(-Wshadow)
  add_definitions(-DDEBUGGING)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG
      "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG
      "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=undefined ")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG
      "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=leak")
endif()

add_definitions(-Werror)

include(CheckIPOSupported)
check_ipo_supported(RESULT HAS_IPO_SUPPORT OUTPUT IPO_ERROR)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIR})
set(Boost_USE_MULTITHREADED ON)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

add_library(
  wayout-player OBJECT
  src/ArgumentParser.cpp
  src/ArgumentParser.hpp
  src/SystemInformation.cpp
  src/SystemInformation.hpp
  src/Board.cpp
  src/Board.hpp
  src/Types.hpp
  src/Position.cpp
  src/Position.hpp
  src/Tile.cpp
  src/Tile.hpp
  src/Text.cpp
  src/Text.hpp
  src/Solver.cpp
  src/Solver.hpp
  src/SolverConfiguration.cpp
  src/SolverConfiguration.hpp
  src/TileType.cpp
  src/TileType.hpp
  src/Filesystem.cpp
  src/Filesystem.hpp
  src/Hashing.cpp
  src/Hashing.hpp
  src/Solution.cpp
  src/Solution.hpp)

find_package(Threads REQUIRED)

add_executable(test test/Tests.cpp $<TARGET_OBJECTS:wayout-player>)
target_link_libraries(test ${OPENSSL_CRYPTO_LIBRARY})
target_link_libraries(test ${OPENSSL_SSL_LIBRARY})
target_link_libraries(test ${Boost_LIBRARIES})
target_link_libraries(test ${CMAKE_THREAD_LIBS_INIT})

add_executable(player src/Player.cpp $<TARGET_OBJECTS:wayout-player>)
target_link_libraries(player ${OPENSSL_CRYPTO_LIBRARY})
target_link_libraries(player ${OPENSSL_SSL_LIBRARY})
target_link_libraries(player ${Boost_LIBRARIES})
target_link_libraries(player ${CMAKE_THREAD_LIBS_INIT})

if(HAS_IPO_SUPPORT)
  message(STATUS "IPO enabled")
  set_property(TARGET player PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(STATUS "IPO not supported: <${IPO_ERROR}>")
endif()
