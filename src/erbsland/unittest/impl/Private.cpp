// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "Private.hpp"


#include "AssertFailed.hpp"
#include "AssertFlags.hpp"
#include "Controller.hpp"
#include "Demangle.hpp"
#include "ErrorCapture.hpp"

#include "../UnitTest.hpp"

#include <sstream>


namespace erbsland::unittest {


void Private::handleAssertResult(
    AssertResult result,
    const AssertContext &context,
    UnitTest *unitTest) {

    ErrorCapturePtr errorCapture;
    if (result == UnexpectedException) {
        errorCapture = Controller::instance()->reportError("UNEXPECTED EXCEPTION!", ConsoleColor::Red);
    } else {
        bool isExpected = (result == ExpectedResult);
        if ((context.flags & AssertNegate) != 0) {
            isExpected = !isExpected;
        }
        if (isExpected) {
            return;
        }
        errorCapture = Controller::instance()->reportError("FAILED!", ConsoleColor::Red);
    }

    std::stringstream text;
    auto console = Controller::instance()->console();
    if (!context.exceptionType.empty()) {
        text << "Exception Type: " << demangleTypeName(context.exceptionType) << "\n"
            << "Exception Message: " << context.exceptionMessage;
        console->writeDebug(text.str());
        errorCapture->addDebugInfo(text.str());
    }

    if (!_contextStack.empty()) {
        text.str({});
        for (std::size_t i = _contextStack.size(); i > 0; --i) {
            if (i != _contextStack.size()) {
                text << "\n";
            }
            text << "[" << i << "]: " << _contextStack[i-1]->toString();
        }
        console->writeErrorInfo(text.str());
        errorCapture->addContextInfo(text.str());
    }

    auto additionalInfo = unitTest->additionalErrorMessages();
    if (!additionalInfo.empty()) {
        console->writeDebug(additionalInfo);
        errorCapture->addDebugInfo(additionalInfo);
    }

    if ((context.flags & AssertCheck) == 0) {
        throw AssertFailed();
    }
}


void Private::addContext(AssertContext *context) noexcept {
    _contextStack.push_back(context);
}


void Private::removeContext(AssertContext *context) noexcept {
    if (_contextStack.empty() || _contextStack.back() != context) {
        auto console = Controller::instance()->console();
        console->writeError("Context stack corruption. Do not use `REQUIRE` macros in sub threads!");
    } else {
        _contextStack.pop_back();
    }
}


}

