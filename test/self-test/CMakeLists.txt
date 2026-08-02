# Copyright (c) 2026 Tobias Erbsland - https://erbsland.dev
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.25)

project(self-test)
add_executable(self-test
        src/CommandLineTest.cpp
        src/ConstructorFailureTest.cpp
        src/EarlyStopTest.cpp
        src/main.cpp
        src/OutputTest.cpp
        src/RunnerFailureTest.cpp
        src/TimingTest.cpp
)
target_compile_features(self-test PRIVATE cxx_std_20)
target_include_directories(self-test PRIVATE ../../src)
erbsland_unittest(TARGET self-test ENABLE_WARNINGS PRECOMPILE_HEADERS)
