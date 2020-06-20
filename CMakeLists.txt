cmake_minimum_required(VERSION 3.12)

project(wayout-player)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

find_package(OpenCV REQUIRED core imgcodecs imgproc)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories("include")

add_library(
  wayout-player OBJECT
  src/Argument.cpp
  src/Argument.hpp
  src/ArgumentFactory.cpp
  src/ArgumentFactory.hpp
  src/ArgumentParser.cpp
  src/ArgumentParser.hpp
  src/Average.hpp
  src/Board.cpp
  src/Board.hpp
  src/BoardComponentSplitter.cpp
  src/BoardComponentSplitter.hpp
  src/BoardScanner.cpp
  src/BoardScanner.hpp
  src/Color.hpp
  src/Filesystem.cpp
  src/Filesystem.hpp
  src/FloatingPointScreenCoordinates.cpp
  src/FloatingPointScreenCoordinates.hpp
  src/Hashing.cpp
  src/Hashing.hpp
  src/Image.cpp
  src/Image.hpp
  src/ImageComponentType.cpp
  src/ImageComponentType.hpp
  src/IntegralScreenCoordinates.cpp
  src/IntegralScreenCoordinates.hpp
  src/Mask.cpp
  src/Mask.hpp
  src/MaskComponentFinder.cpp
  src/MaskComponentFinder.hpp
  src/Numeric.hpp
  src/Position.cpp
  src/Position.hpp
  src/Range.hpp
  src/Solution.cpp
  src/Solution.hpp
  src/Solver.cpp
  src/Solver.hpp
  src/SolverConfiguration.cpp
  src/SolverConfiguration.hpp
  src/SystemInformation.cpp
  src/SystemInformation.hpp
  src/Text.cpp
  src/Text.hpp
  src/Tile.cpp
  src/Tile.hpp
  src/TileType.cpp
  src/TileType.hpp
  src/Types.hpp)

find_package(Threads REQUIRED)

list(APPEND WAYOUT_PLAYER_LINK_LIBRARIES ${OPENSSL_CRYPTO_LIBRARY})
list(APPEND WAYOUT_PLAYER_LINK_LIBRARIES ${OPENSSL_SSL_LIBRARY})
list(APPEND WAYOUT_PLAYER_LINK_LIBRARIES ${Boost_LIBRARIES})
list(APPEND WAYOUT_PLAYER_LINK_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
list(APPEND WAYOUT_PLAYER_LINK_LIBRARIES ${OpenCV_LIBS})

add_executable(player src/Player.cpp $<TARGET_OBJECTS:wayout-player>)
target_link_libraries(player ${WAYOUT_PLAYER_LINK_LIBRARIES})

add_library(
  wayout-player-tests OBJECT
  test/BoardComponentSplitterTests.cpp
  test/BoardComponentSplitterTests.hpp
  test/BoardTests.cpp
  test/BoardTests.hpp
  test/ColorTests.cpp
  test/ColorTests.hpp
  test/HashingTests.cpp
  test/HashingTests.hpp
  test/ImageTests.cpp
  test/ImageTests.hpp
  test/MaskComponentFinderTests.cpp
  test/MaskComponentFinderTests.hpp
  test/PositionTests.cpp
  test/PositionTests.hpp
  test/RangeTests.cpp
  test/RangeTests.hpp
  test/SolverTests.cpp
  test/SolverTests.hpp
  test/TextTests.cpp
  test/TextTests.hpp
  test/TileTypeTests.cpp
  test/TileTypeTests.hpp)

add_executable(test test/Tests.cpp $<TARGET_OBJECTS:wayout-player>
                    $<TARGET_OBJECTS:wayout-player-tests>)
target_link_libraries(test ${WAYOUT_PLAYER_LINK_LIBRARIES})

if(HAS_IPO_SUPPORT)
  message(STATUS "IPO enabled")
  set_property(TARGET player PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(STATUS "IPO not supported: <${IPO_ERROR}>")
endif()
