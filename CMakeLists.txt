# Copyright © 2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch/
# According to the copyright terms specified in the file "COPYRIGHT.md".
# SPDX-License-Identifier: LGPL-3.0-or-later

cmake_minimum_required(VERSION 3.20)

project(erbsland-unittest)
add_library(erbsland-unittest STATIC)
set_target_properties(erbsland-unittest PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(erbsland-unittest PRIVATE cxx_std_20)
target_include_directories(erbsland-unittest INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")

add_subdirectory(src/erbsland/unittest)

include(tools/unittest/unittest.cmake)

