# Copyright (c) 2026 Tobias Erbsland - https://erbsland.dev
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.25)

target_sources(erbsland-unittest PRIVATE
        AssertContext.cpp
        AssertContext.hpp
        AssertFailed.hpp
        AssertFlags.hpp
        AssertResult.hpp
        Console.cpp
        Console.hpp
        ConsoleColor.cpp
        ConsoleColor.hpp
        ConsoleLine.cpp
        ConsoleLine.hpp
        Controller.cpp
        Controller.hpp
        Definitions.hpp
        Demangle.cpp
        Demangle.hpp
        ErrorCapture.cpp
        ErrorCapture.hpp
        Filter.hpp
        Macros.hpp
        MetaData.cpp
        MetaData.hpp
        Private.cpp
        Private.hpp
        Registration.hpp
        SourceLocation.hpp
        Test.hpp
        TestBase.cpp
        TestBase.hpp
        TestClass.hpp
        TestClassBase.cpp
        TestClassBase.hpp
        TextHelperImpl.cpp
        TextHelperImpl.hpp
)
