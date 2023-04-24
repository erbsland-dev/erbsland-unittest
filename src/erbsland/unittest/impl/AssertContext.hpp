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

