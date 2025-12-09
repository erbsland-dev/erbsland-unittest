// Copyright © 2023-2025 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "Private.hpp"


// make sure we have no conflicts.
#undef ADD_TEST
#undef ADD_CLASS
#undef REQUIRE
#undef REQUIRE_FALSE
#undef REQUIRE_THROWS
#undef REQUIRE_THROWS_AS
#undef REQUIRE_NOTHROW
#undef REQUIRE_EQUAL
#undef REQUIRE_NOT_EQUAL
#undef REQUIRE_LESS
#undef REQUIRE_LESS_EQUAL
#undef REQUIRE_GREATER
#undef REQUIRE_GREATER_EQUAL
#undef CHECK
#undef CHECK_FALSE
#undef CHECK_THROWS
#undef CHECK_THROWS_AS
#undef CHECK_NOTHROW
#undef CHECK_EQUAL
#undef CHECK_NOT_EQUAL
#undef CHECK_LESS
#undef CHECK_LESS_EQUAL
#undef CHECK_GREATER
#undef CHECK_GREATER_EQUAL
#undef UNITTEST_SUBCLASS


#define ASSERT_CONTEXT_REQUIRE(macroName, flags, ...) \
    ::erbsland::unittest::require(this, flags, macroName, #__VA_ARGS__, {__FILE__, __LINE__}, [&]() -> bool { \
        return static_cast<bool>(__VA_ARGS__); \
    });
#define ASSERT_CONTEXT_COMPARISON(macroName, flags, op, a, b) \
    ::erbsland::unittest::requireComparison(this, flags, macroName, #a #op #b, {__FILE__, __LINE__}, [&]() -> bool { \
        return static_cast<bool>((a) op (b)); \
    }, [&]() -> std::string { \
        return ::erbsland::unittest::comparisonErrorMessage(#op, #a, #b, (a), (b)); \
    });
#define ASSERT_CONTEXT_NOTHROW(macroName, flags, ...) \
    ::erbsland::unittest::requireNoThrow(this, flags, macroName, #__VA_ARGS__, {__FILE__, __LINE__}, [&]() -> void { \
        static_cast<void>(__VA_ARGS__); \
    });
#define ASSERT_CONTEXT_THROWS(macroName, flags, ...) \
    ::erbsland::unittest::requireThrows(this, flags, macroName, #__VA_ARGS__, {__FILE__, __LINE__}, [&]() -> void { \
        static_cast<void>(__VA_ARGS__); \
    });
#define ASSERT_CONTEXT_THROWS_AS(macroName, flags, exceptionClass, ...) \
    ::erbsland::unittest::requireThrowsAs<exceptionClass>(this, flags, macroName, #exceptionClass ", " #__VA_ARGS__, {__FILE__, __LINE__}, [&]() -> void { \
        static_cast<void>(__VA_ARGS__); \
    });
// run an expression, but add context information to it.
#define WITH_CONTEXT( ... ) \
    ::erbsland::unittest::runWithContext(this, 0, "WITH_CONTEXT", #__VA_ARGS__, {__FILE__, __LINE__}, [&]() -> void { \
        static_cast<void>(__VA_ARGS__); \
    });
// get the run context for the function `runWithContext`.
#define SOURCE_LOCATION() (::erbsland::unittest::SourceLocation{__FILE__, __LINE__})

#define REQUIRE( ... ) ASSERT_CONTEXT_REQUIRE("REQUIRE", 0, __VA_ARGS__)
#define REQUIRE_FALSE( ... ) ASSERT_CONTEXT_REQUIRE("REQUIRE_FALSE", (::erbsland::unittest::AssertNegate), __VA_ARGS__)
#define REQUIRE_THROWS( ... ) ASSERT_CONTEXT_THROWS("REQUIRE_THROWS", 0, __VA_ARGS__)
#define REQUIRE_THROWS_AS(exceptionClass, ... ) ASSERT_CONTEXT_THROWS_AS("REQUIRE_THROWS_AS", 0, exceptionClass, __VA_ARGS__)
#define REQUIRE_NOTHROW( ... ) ASSERT_CONTEXT_NOTHROW("REQUIRE_NOTHROW", 0, __VA_ARGS__)
#define REQUIRE_EQUAL(a, b) ASSERT_CONTEXT_COMPARISON("REQUIRE_EQUAL", 0, ==, a, b)
#define REQUIRE_NOT_EQUAL(a, b) ASSERT_CONTEXT_COMPARISON("REQUIRE_NOT_EQUAL", 0, !=, a, b)
#define REQUIRE_LESS(a, b) ASSERT_CONTEXT_COMPARISON("REQUIRE_LESS", 0, <, a, b)
#define REQUIRE_LESS_EQUAL(a, b) ASSERT_CONTEXT_COMPARISON("REQUIRE_LESS_EQUAL", 0, <=, a, b)
#define REQUIRE_GREATER(a, b) ASSERT_CONTEXT_COMPARISON("REQUIRE_GREATER", 0, >, a, b)
#define REQUIRE_GREATER_EQUAL(a, b) ASSERT_CONTEXT_COMPARISON("REQUIRE_GREATER_EQUAL", 0, >=, a, b)

#define CHECK( ... ) ASSERT_CONTEXT_REQUIRE("CHECK", (::erbsland::unittest::AssertCheck), __VA_ARGS__)
#define CHECK_FALSE( ... ) ASSERT_CONTEXT_REQUIRE("CHECK_FALSE", (::erbsland::unittest::AssertNegate|::erbsland::unittest::AssertCheck), __VA_ARGS__)
#define CHECK_THROWS( ... ) ASSERT_CONTEXT_THROWS("CHECK_THROWS", (::erbsland::unittest::AssertCheck), __VA_ARGS__)
#define CHECK_THROWS_AS(exceptionClass, ... ) ASSERT_CONTEXT_THROWS_AS("CHECK_THROWS_AS", (::erbsland::unittest::AssertCheck), exceptionClass, __VA_ARGS__)
#define CHECK_NOTHROW( ... ) ASSERT_CONTEXT_NOTHROW("CHECK_NOTHROW", (::erbsland::unittest::AssertCheck), __VA_ARGS__)
#define CHECK_EQUAL(a, b) ASSERT_CONTEXT_COMPARISON("CHECK_EQUAL", (::erbsland::unittest::AssertCheck), ==, a, b)
#define CHECK_NOT_EQUAL(a, b) ASSERT_CONTEXT_COMPARISON("CHECK_NOT_EQUAL", (::erbsland::unittest::AssertCheck), !=, a, b)
#define CHECK_LESS(a, b) ASSERT_CONTEXT_COMPARISON("CHECK_LESS", (::erbsland::unittest::AssertCheck), <, a, b)
#define CHECK_LESS_EQUAL(a, b) ASSERT_CONTEXT_COMPARISON("CHECK_LESS_EQUAL", (::erbsland::unittest::AssertCheck), <=, a, b)
#define CHECK_GREATER(a, b) ASSERT_CONTEXT_COMPARISON("CHECK_GREATER", (::erbsland::unittest::AssertCheck), >, a, b)
#define CHECK_GREATER_EQUAL(a, b) ASSERT_CONTEXT_COMPARISON("CHECK_GREATER_EQUAL", (::erbsland::unittest::AssertCheck), >=, a, b)

/// Begin: Manual test registration.
///
#define TESTS_BEGIN(class_name) \
    public: \
    static constexpr std::tuple<void (class_name::*)(), const char*> cTestFunctions[] = {

/// End: Manual test registration.
///
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
    auto main(int argc, char *argv[]) -> int { \
        return ::erbsland::unittest::Controller::instance()->main(argc, argv); \
    }

