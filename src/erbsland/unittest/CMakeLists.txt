# Copyright (c) 2026 Tobias Erbsland - https://erbsland.dev
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.25)

add_subdirectory(impl)

target_sources(erbsland-unittest PRIVATE
        all.hpp
        FileHelper.cpp
        FileHelper.hpp
        fwd.hpp
        TextHelper.cpp
        TextHelper.hpp
        UnitTest.cpp
        UnitTest.hpp
)
