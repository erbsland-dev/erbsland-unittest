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
        errorCapture = Controller::instance()->reportError("UNEXPECTED EXCEPTION!", Console::Red);
    } else {
        bool isExpected = (result == ExpectedResult);
        if ((context.flags & AssertNegate) != 0) {
            isExpected = !isExpected;
        }
        if (isExpected) {
            return;
        }
        errorCapture = Controller::instance()->reportError("FAILED!", Console::Red);
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

