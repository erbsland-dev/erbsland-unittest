# Copyright © 2023-2025 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch/
# According to the copyright terms specified in the file "COPYRIGHT.md".
# SPDX-License-Identifier: LGPL-3.0-or-later

cmake_minimum_required(VERSION 3.23)

project(erbsland-unittest)
option(ERBSLAND_UNITTEST_ENABLE_TESTS "Enable self-tests" OFF)

# Create the static library
add_library(erbsland-unittest STATIC)

# Add basic properties
set_target_properties(erbsland-unittest PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(erbsland-unittest PUBLIC cxx_std_20)

# Set flags and warnings for various compilers.
if(MSVC)
    target_compile_options(erbsland-unittest PUBLIC "/utf-8")
endif()

if(DEFINED _DEBUG OR DEFINED DEBUG)
    if(MSVC)
        target_compile_options(erbsland-unittest PRIVATE "/W4" "/WX")
    else()
        target_compile_options(erbsland-unittest PRIVATE "-Wall" "-Wextra" "-Werror")
        if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            target_compile_options(erbsland-unittest PRIVATE "-fno-omit-frame-pointer" "-mno-omit-leaf-frame-pointer")
        endif ()
    endif()
endif()

# Add the sources.
add_subdirectory(src/erbsland/unittest)

# Import the tools.
include(tools/unittest/unittest.cmake)

# Add the include path for integrations and installations
target_include_directories(erbsland-unittest
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

include(install.cmake)
include(tests.cmake)
