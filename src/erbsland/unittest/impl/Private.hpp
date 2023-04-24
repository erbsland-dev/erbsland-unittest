﻿// Copyright © 2023 Tobias Erbsland. Web: https://erbsland.dev
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


#include "AssertContext.hpp"
#include "AssertFailed.hpp"
#include "AssertFlags.hpp"
#include "AssertResult.hpp"
#include "Filter.hpp"
#include "MetaData.hpp"
#include "Registration.hpp"
#include "Test.hpp"


/// A minimalistic unittest system to allow *dependency free* tests of the library itself.
///
/// The system has no dependencies to the actual library, and the two files `UnitTest.hpp` and `UnitTest.cpp`,
/// with all implementation files in the folder `unittest/` can be used independently of the library.
///
/// There is a CMake extension that uses a Python script to automatically extract meta information from the
/// test declarations and write repetitive support code to run the tests. There is also an alternative
/// manual way to write all required registrations, without python scripts and CMake needed. The manual way
/// is not recommended, as it makes maintaining a lange number of unittest difficult if the system changes.
///
/// For a proper function of the unittest system, quite a number of preprocessor macros are required as the
/// C++ language provides no reflection for source code files and lines numbers.
///
/// In order to keep the unittests simple and small, there are no prefixes for these macros (e.g. `REQUIRE`).
/// Previously existing macros with the same names are overwritten.
///
/// For this reason:
/// - Use this file only for a dedicated unittest executable as demonstrated with the tests for this library.
///   See repository `ErbslandCoreUnitTest` for the tests.
/// - Add the `#include <erbsland/UnitTest.hpp>` as the last include statement in the file!
///
namespace erbsland::unittest {


class UnitTest;


/// @internal
/// A struct to encapsulate the private implementation avoiding name conflicts.
///
class Private {
public: // runtime methods
    /// Handle the result of the assert evaluation.
    ///
    /// @param result The result of the handler.
    /// @param context The context.
    /// @param unitTest The current unittest.
    ///
    void handleAssertResult(
        AssertResult result,
        const AssertContext &context,
        UnitTest *unitTest);

    /// Add a context to the stack.
    ///
    void addContext(AssertContext *context) noexcept;

    /// Remove a context from the stack.
    ///
    void removeContext(AssertContext *context) noexcept;

private:
    std::vector<AssertContext*> _contextStack; ///< A stack with the nested contexts.
};


}

