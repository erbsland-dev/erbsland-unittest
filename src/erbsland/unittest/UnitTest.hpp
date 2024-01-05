// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "impl/AssertContext.hpp"
#include "impl/Macros.hpp"
#include "impl/Private.hpp"

#include "impl/Definitions.hpp"

#include <filesystem>


namespace erbsland::unittest {


/// A minimal unit test system to write tests for the library.
///
/// You can use it for your own unittests, but be aware of the potential naming
/// conflicts because of all the macros used for the tests. Make sure you add `#include <erbsland/UnitTest.h>`
/// as the last include statement in your unittest file!
///
/// See the existing unit tests for the core library how to use this unit test class.
///
/// **Thread Safety:** The unit tests are meant to run in a single thread. If tests use multi threaded testing,
/// macros like `REQUIRE()` must only be called from the main thread.
///
class UnitTest {
    // fwd-entry: class UnitTest

public:
    /// Create a new unittest instance.
    ///
    UnitTest() = default;

    /// dtor
    ///
    virtual ~UnitTest() = default;

public:
    /// Add additional output to the error message.
    ///
    /// Overwrite this method to add additional text after the error message.
    ///
    /// @return The additional text that is added to the error message. May contain newlines.
    ///
    virtual auto additionalErrorMessages() -> std::string;

    /// Execute code before every test in this class.
    ///
    virtual void setUp();

    /// Execute code after every test in this class.
    ///
    virtual void tearDown();

public: // helper methods.
    /// Run a code in a separate context and optionally collect additional information if a tests is failing.
    ///
    /// Use this function like this:
    /// ```
    /// int x = 5;
    /// runWithContext(SOURCE_LOCATION(), [&]() {
    ///     x = 9;
    ///     REQUIRE(x == 10);
    /// }, [&]() {
    ///     std::stringstream text;
    ///     text << "x = " << x;
    ///     return text.str();
    /// });
    /// ```
    ///
    /// @param sourceLocation Use the `SOURCE_LOCATION()` macro for this parameter.
    /// @param testFn The lambda function with the tests.
    /// @param diagnoseFn Optional function to collect diagnose information.
    ///
    void runWithContext(
        const SourceLocation &sourceLocation,
        const std::function<void()> &testFn,
        const std::function<std::string()> &diagnoseFn = nullptr);

    /// Method to write debug messages to the console.
    ///
    /// A newline will be added automatically.
    ///
    /// @param text The text to write to the console.
    ///
    void consoleWriteLine(const std::string &text);

    /// Access the executable path for the unittest executable.
    ///
    /// @return The absolute path to the currently executed unittest executable.
    ///
    auto unitTestExecutablePath() -> std::filesystem::path;

public:
    friend class Private;
    friend class AssertContext;

public:
    Private p; ///< @private The private implementation
};


}

