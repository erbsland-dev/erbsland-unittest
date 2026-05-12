# Copyright (c) 2026 Tobias Erbsland - https://erbsland.dev
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.23)

project(erbsland-unittest)
option(ERBSLAND_UNITTEST_ENABLE_TESTS "Enable self-tests" OFF)

# Load our functions for setting compiler flags
include(cmake/debug-warnings.cmake)
include(cmake/compiler-options.cmake)

# Create the static library
add_library(erbsland-unittest STATIC)

# Add basic properties
set_target_properties(erbsland-unittest PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(erbsland-unittest PUBLIC cxx_std_20)

# Set flags and warnings for various compilers.
erbsland_unittest_set_required_compiler_options(erbsland-unittest)
erbsland_unittest_enable_debug_warnings(erbsland-unittest)

# Add the sources.
add_subdirectory(src/erbsland/unittest)

# Import the tools.
include(cmake/unittest.cmake)

# Add the include path for integrations and installations
target_include_directories(erbsland-unittest
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

include(cmake/install.cmake)
include(cmake/tests.cmake)
