#!/usr/bin/env bash

LLVM_PROFILE_FILE="test.profraw" ./test

llvm-profdata merge -sparse test.profraw -o test.profdata

# llvm-cov show ./test -instr-profile=test.profdata

llvm-cov report ./test -instr-profile=test.profdata ../src/*

rm test.profdata test.profraw
