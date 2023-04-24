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
    return Controller::instance()->executablePath();
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

