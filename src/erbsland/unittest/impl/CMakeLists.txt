# Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch/
# According to the copyright terms specified in the file "COPYRIGHT.md".
# SPDX-License-Identifier: LGPL-3.0-or-later

cmake_minimum_required(VERSION 3.20)

target_sources(erbsland-unittest PRIVATE
        AssertContext.cpp
        AssertContext.hpp
        AssertFailed.hpp
        AssertFlags.hpp
        AssertResult.hpp
        Console.cpp
        Console.hpp
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
)

