#!/usr/bin/env bash

# Run this script from within your build tree.

# Format the source code.
cmake-format -i ../CMakeLists.txt
clang-format -i ../src/* ../test/*

# Run CMake.
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..

# Specify the configuration you want here.
cmake --build . --config Release --parallel 8

# Run the test suite.
./test --report_level=detailed --log_level=all
