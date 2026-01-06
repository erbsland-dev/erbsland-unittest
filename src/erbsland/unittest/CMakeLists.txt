# Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch/
# According to the copyright terms specified in the file "COPYRIGHT.md".
# SPDX-License-Identifier: LGPL-3.0-or-later

cmake_minimum_required(VERSION 3.20)

target_sources(erbsland-unittest PRIVATE
        all.hpp
        fwd.hpp
        FileHelper.cpp
        FileHelper.hpp
        UnitTest.cpp
        UnitTest.hpp
)

add_subdirectory(impl)

