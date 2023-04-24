// Copyright © 2023 Tobias Erbsland. Web: https://erbsland.dev
// Copyright © 2023 EducateIT GmbH. Web: https://educateit.ch
//
// This program is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with this program.
// If not, see <https://www.gnu.org/licenses/>.
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


#define ASSERT_CONTEXT_REQUIRE(macroName, flags, ...) \
    do { \
        ::erbsland::unittest::AssertContext erbsland_unittest_assert_context(this, flags, macroName, #__VA_ARGS__, {__FILE__, __LINE__}); \
        try { \
            if (__VA_ARGS__) { \
                erbsland_unittest_assert_context.expectedResult(); \
            } else { \
                erbsland_unittest_assert_context.unexpectedResult(); \
            } \
        } catch (const ::erbsland::unittest::AssertFailed&) { \
            throw; \
        } catch (const std::exception &ex) { \
            erbsland_unittest_assert_context.exceptionType = std::string(typeid(ex).name()); \
            erbsland_unittest_assert_context.exceptionMessage = std::string(ex.what()); \
            erbsland_unittest_assert_context.unexpectedException(); \
        } catch (...) { \
            erbsland_unittest_assert_context.unexpectedException(); \
        } \
    } while(false);
#define ASSERT_CONTEXT_NOTHROW(macroName, flags, ...) \
    do { \
        ::erbsland::unittest::AssertContext erbsland_unittest_assert_context(this, flags, macroName, #__VA_ARGS__, {__FILE__, __LINE__}); \
        try { \
            static_cast<void>(__VA_ARGS__); \
            erbsland_unittest_assert_context.expectedResult(); \
        } catch (const ::erbsland::unittest::AssertFailed&) { \
            throw; \
        } catch (const std::exception &ex) { \
            erbsland_unittest_assert_context.exceptionType = std::string(typeid(ex).name()); \
            erbsland_unittest_assert_context.exceptionMessage = std::string(ex.what()); \
            erbsland_unittest_assert_context.unexpectedResult(); \
        } catch (...) { \
            erbsland_unittest_assert_context.unexpectedResult(); \
        } \
    } while(false);
#define ASSERT_CONTEXT_THROWS(macroName, flags, ...) \
    do { \
        ::erbsland::unittest::AssertContext erbsland_unittest_assert_context(this, flags, macroName, #__VA_ARGS__, {__FILE__, __LINE__}); \
        try { \
            static_cast<void>(__VA_ARGS__); \
            erbsland_unittest_assert_context.unexpectedResult(); \
        } catch (const ::erbsland::unittest::AssertFailed&) { \
            throw; \
        } catch (...) { \
            erbsland_unittest_assert_context.expectedResult(); \
        } \
    } while(false);
#define ASSERT_CONTEXT_THROWS_AS(macroName, flags, exceptionClass, ...) \
    do { \
        ::erbsland::unittest::AssertContext erbsland_unittest_assert_context(this, flags, macroName, #exceptionClass ", " #__VA_ARGS__, {__FILE__, __LINE__}); \
        try { \
            static_cast<void>(__VA_ARGS__); \
            erbsland_unittest_assert_context.unexpectedResult(); \
        } catch (const exceptionClass &ex) { \
            erbsland_unittest_assert_context.expectedResult(); \
        } catch (const ::erbsland::unittest::AssertFailed&) { \
            throw; \
        } catch (const std::exception &ex) { \
            erbsland_unittest_assert_context.exceptionType = std::string(typeid(ex).name()); \
            erbsland_unittest_assert_context.exceptionMessage = std::string(ex.what()); \
            erbsland_unittest_assert_context.unexpectedException(); \
        } catch (...) { \
            erbsland_unittest_assert_context.unexpectedException(); \
        } \
    } while(false);
// run an expression, but add context information to it.
#define WITH_CONTEXT( ... ) \
    do { \
        ::erbsland::unittest::AssertContext erbsland_unittest_assert_context(this, 0, "WITH_CONTEXT", #__VA_ARGS__, {__FILE__, __LINE__}); \
        try { \
            (__VA_ARGS__); \
        } catch(const ::erbsland::unittest::AssertFailed&) { \
            throw; \
        } catch (const std::exception &ex) { \
            erbsland_unittest_assert_context.exceptionType = std::string(typeid(ex).name()); \
            erbsland_unittest_assert_context.exceptionMessage = std::string(ex.what()); \
            erbsland_unittest_assert_context.unexpectedException(); \
        } catch(...) { \
            erbsland_unittest_assert_context.unexpectedException(); \
        } \
    } while(false);
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

