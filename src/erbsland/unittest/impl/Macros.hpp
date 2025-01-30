﻿// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "Private.hpp"


// make sure we have no conflicts.
#undef assert
#undef ADD_TEST
#undef ADD_CLASS
#undef REQUIRE
#undef REQUIRE_FALSE
#undef REQUIRE_THROWS
#undef REQUIRE_THROWS_AS
#undef REQUIRE_NOTHROW
#undef CHECK
#undef CHECK_FALSE
#undef CHECK_THROWS
#undef CHECK_THROWS_AS
#undef CHECK_NOTHROW
#undef UNITTEST_SUBCLASS


#define ASSERT_CONTEXT_REQUIRE(macroName, flags, ...) \
    ::erbsland::unittest::AssertContext{ \
        this, flags, macroName, #__VA_ARGS__, {__FILE__, __LINE__} \
    }.require([&, this]() { \
        return (__VA_ARGS__); \
    });
#define ASSERT_CONTEXT_NOTHROW(macroName, flags, ...) \
    ::erbsland::unittest::AssertContext{ \
        this, flags, macroName, #__VA_ARGS__, {__FILE__, __LINE__} \
    }.requireNoThrow([&, this]() { \
        static_cast<void>(__VA_ARGS__); \
    });
#define ASSERT_CONTEXT_THROWS(macroName, flags, ...) \
    ::erbsland::unittest::AssertContext{ \
        this, flags, macroName, #__VA_ARGS__, {__FILE__, __LINE__} \
    }.requireThrows([&, this]() { \
        static_cast<void>(__VA_ARGS__); \
    });
#define ASSERT_CONTEXT_THROWS_AS(macroName, flags, exceptionClass, ...) \
    ::erbsland::unittest::AssertContext{ \
        this, flags, macroName, #exceptionClass ", " #__VA_ARGS__, {__FILE__, __LINE__} \
    }.requireThrowsAs<exceptionClass>([&, this]() { \
        static_cast<void>(__VA_ARGS__); \
    });
// run an expression, but add context information to it.
#define WITH_CONTEXT( ... ) \
    ::erbsland::unittest::AssertContext{ \
        this, 0, "WITH_CONTEXT", #__VA_ARGS__, {__FILE__, __LINE__} \
    }.runWithContext([&, this]() { \
        static_cast<void>(__VA_ARGS__); \
    });
// get the run context for the function `runWithContext`.
#define SOURCE_LOCATION() (::erbsland::unittest::SourceLocation{__FILE__, __LINE__})

#define REQUIRE( ... ) ASSERT_CONTEXT_REQUIRE("REQUIRE", 0, __VA_ARGS__)
#define REQUIRE_FALSE( ... ) ASSERT_CONTEXT_REQUIRE("REQUIRE_FALSE", (::erbsland::unittest::AssertNegate), __VA_ARGS__)
#define REQUIRE_THROWS( ... ) ASSERT_CONTEXT_THROWS("REQUIRE_THROWS", 0, __VA_ARGS__)
#define REQUIRE_THROWS_AS(exceptionClass, ... ) ASSERT_CONTEXT_THROWS_AS("REQUIRE_THROWS_AS", 0, exceptionClass, __VA_ARGS__)
#define REQUIRE_NOTHROW( ... ) ASSERT_CONTEXT_NOTHROW("REQUIRE_NOTHROW", 0, __VA_ARGS__)

#define CHECK( ... ) ASSERT_CONTEXT_REQUIRE("CHECK", (::erbsland::unittest::AssertCheck), __VA_ARGS__)
#define CHECK_FALSE( ... ) ASSERT_CONTEXT_REQUIRE("CHECK_FALSE", (::erbsland::unittest::AssertNegate|::erbsland::unittest::AssertCheck), __VA_ARGS__)
#define CHECK_THROWS( ... ) ASSERT_CONTEXT_THROWS("CHECK_THROWS", (::erbsland::unittest::AssertCheck), __VA_ARGS__)
#define CHECK_THROWS_AS(exceptionClass, ... ) ASSERT_CONTEXT_THROWS_AS("CHECK_THROWS_AS", (::erbsland::unittest::AssertCheck), exceptionClass, __VA_ARGS__)
#define CHECK_NOTHROW( ... ) ASSERT_CONTEXT_NOTHROW("CHECK_NOTHROW", (::erbsland::unittest::AssertCheck), __VA_ARGS__)

/// The begin to manually registers tests.
///
#define TESTS_BEGIN(class_name) \
    public: \
    static constexpr std::tuple<void (class_name::*)(), const char*> cTestFunctions[] = {

#define TESTS_END() \
    };

/// Add a test to the current unit test class.
///
/// Use this method in the constructor of your unit test to add all test methods.
///
#define TESTS_ENTRY(function_name) \
    std::make_tuple((&function_name), #function_name),

/// Add a test to the current unit test class.
///
/// Use this method in the constructor of your unit test to add all test methods.
///
#define ADD_TEST(function_name) \
    p.addTest((&function_name), std::string(#function_name));

/// Register a unit test class.
///
/// Add this after your unit test class to automatically register it.
///
#define ADD_CLASS(class_name) \
    ::erbsland::unittest::ManualRegistration<class_name> test_registration ## class_name{#class_name};

/// Mark a class as subclass from `UnitTest`
///
#define UNITTEST_SUBCLASS(class_name) class_name

/// Define an empty TAGS() macro, to safely mark the class and method with tags.
///
#define TAGS(tags)

/// Define an empty TESTED_TARGETS() macro, allow adding a list of tested classes or functions.
///
#define TESTED_TARGETS(name_list)

/// An empty macro to skip the test or class unless the tag is explicitly specified.
///
#define SKIP_BY_DEFAULT()

/// Define the main method for the unit test executable.
///
/// Create a file `main.cpp` with this macro to define the main method for the unit test.
///
#define ERBSLAND_UNITTEST_MAIN() \
    int main(int argc, char *argv[]) { \
        return ::erbsland::unittest::Controller::instance()->main(argc, argv); \
    }

