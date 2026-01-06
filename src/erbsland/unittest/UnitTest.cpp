// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "UnitTest.hpp"


namespace erbsland::unittest {


auto UnitTest::additionalErrorMessages() -> std::string {
    return {};
}


void UnitTest::setUp() {
    // empty
}


void UnitTest::tearDown() {
    // empty
}


auto UnitTest::unitTestExecutablePath() -> std::filesystem::path {
    return fh::unitTestExecutablePath();
}


void UnitTest::runWithContext(
    const SourceLocation &sourceLocation,
    const std::function<void()> &testFn,
    const std::function<std::string()> &diagnoseFn) {

    AssertContext context(this, 0, "runWithContext", "", sourceLocation);
    try {
        testFn();
    } catch(const ::erbsland::unittest::AssertFailed&) {
        if (diagnoseFn != nullptr) {
            Controller::instance()->console()->writeDebug(diagnoseFn());
        }
        throw;
    } catch (const std::exception &ex) {
        context.exceptionType = std::string(typeid(ex).name());
        context.exceptionMessage = std::string(ex.what());
        context.unexpectedException();
    } catch(...) { \
        context.unexpectedException();
    }
}


void UnitTest::consoleWriteLine(const std::string &text) {
    Controller::instance()->writeFromUnitTest(text);
}



}

