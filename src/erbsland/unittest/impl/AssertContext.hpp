// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "SourceLocation.hpp"

#include <functional>
#include <string>


namespace erbsland::unittest {


class UnitTest;


/// @internal
/// The context for an assert (require, check) evaluation.
///
class AssertContext
{
public:
    /// ctor
    ///
    AssertContext(
        UnitTest *unitTest,
        int flags,
        const char* macroName,
        const char* expression,
        SourceLocation sourceLocation);

    /// dtor
    ///
    ~AssertContext();

public:
    void expectedResult();
    void unexpectedResult();
    void unexpectedException();

    /// Get this context as string.
    ///
    auto toString() -> std::string;

public:
    UnitTest* unitTest; ///< Unittest for this context.
    int flags; ///< The flags.
    const char *macroName; ///< The current name of the macro evaluating an expression.
    const char *expression; ///< The current expression which was evaluated.
    SourceLocation sourceLocation; ///< The source location.
    std::string exceptionType; ///< The type of the exception
    std::string exceptionMessage; ///< The `what()` message of the exception.
};


}

