// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
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

