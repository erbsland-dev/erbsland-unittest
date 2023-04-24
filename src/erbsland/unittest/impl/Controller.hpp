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


#include "Console.hpp"
#include "ErrorCapture.hpp"
#include "Filter.hpp"

#include <filesystem>
#include <list>
#include <string>
#include <vector>


namespace erbsland::unittest {


class TestClassBase;


/// @internal
/// The unittest controller.
///
class Controller {
public:
    /// ctor
    ///
    Controller() noexcept;

    /// dtor
    ///
    ~Controller();

public:
    /// Run all registered unit tests.
    ///
    auto main(int argc, char *argv[]) -> int;

    /// Add an instance of a new unit test to the global list.
    ///
    /// Use the macro `ADD_CLASS(Example)` after your unit test class to register it.
    ///
    /// @param testClass The test class to add.
    ///
    void addTestClass(TestClassBase *testClass) noexcept;

    /// Access the executable path.
    ///
    auto executablePath() -> std::filesystem::path;

    /// Write messages from the unittest.
    ///
    void writeFromUnitTest(const std::string &text);

    /// Report an error.
    ///
    auto reportError(const std::string &result, Console::Color textColor) -> ErrorCapturePtr;

    /// Access the console.
    ///
    [[nodiscard]] auto console() const noexcept -> Console*;

private:
    /// Parse the command line arguments.
    ///
    auto parseCommandLine(int argc, char *argv[]) -> int;

    /// Print help on the command line.
    ///
    void printHelp();

    /// Print a list of all suites and tests.
    ///
    void printList();

public:
    static auto instance() -> Controller*;

private:
    Console *_console; ///< The console interface.
    std::vector<TestClassBase*> _testClasses{}; ///< The registered test classes.
    std::filesystem::path _executablePath{}; ///< The path with the unit test executable.
    Filter _filter{}; ///< The filter from command line arguments.
    bool _verbose{false}; ///< Enable verbose messages.
    bool _stopAtFirstError{false}; ///< If the unit test shall stop at the first error.
    bool _showSummary{true}; ///< Flag if the summary with the last three errors is displayed.
    bool _waitAfterEachTest{false}; ///< Wait a second after each test.

    std::string _currentSuite; ///< The current suite that is running.
    std::string _currentTest; ///< The current test that is running.
    bool _printMethodRunning{false}; ///< Flag while a print method is running.
    std::list<ErrorCapturePtr> _capturedErrors; ///< The list with captured errors.
};


}

