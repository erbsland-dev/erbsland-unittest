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
#include "AssertContext.hpp"


#include "Private.hpp"

#include "../UnitTest.hpp"

#include <iomanip>
#include <sstream>


namespace erbsland::unittest {


AssertContext::AssertContext(
    UnitTest *unitTest,
    int flags,
    const char *macroName,
    const char *expression,
    SourceLocation sourceLocation)
:
    unitTest(unitTest),
    flags(flags),
    macroName(macroName),
    expression(expression),
    sourceLocation(sourceLocation)
{
    unitTest->p.addContext(this);
}


AssertContext::~AssertContext() {
    unitTest->p.removeContext(this);
}


void AssertContext::expectedResult() {
    unitTest->p.handleAssertResult(ExpectedResult, *this, unitTest);
}


void AssertContext::unexpectedResult() {
    unitTest->p.handleAssertResult(UnexpectedResult, *this, unitTest);
}


void AssertContext::unexpectedException() {
    unitTest->p.handleAssertResult(UnexpectedException, *this, unitTest);
}


auto AssertContext::toString() -> std::string {
    std::stringstream text;
    if (sourceLocation.file != nullptr) {
        text << sourceLocation.file << ":" << std::dec << sourceLocation.lineNo << ": ";
    }
    if (macroName != nullptr && expression != nullptr) {
        text << macroName << "(" << expression << ")";
    }
    return text.str();
}


}

