# Copyright (c) 2023-2026 Tobias Erbsland - https://erbsland.dev
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20)

target_sources(erbsland-unittest PRIVATE
        all.hpp
        fwd.hpp
        FileHelper.cpp
        FileHelper.hpp
        TextHelper.cpp
        TextHelper.hpp
        UnitTest.cpp
        UnitTest.hpp
)

add_subdirectory(impl)

