// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "Controller.hpp"


#include "AssertFailed.hpp"
#include "Demangle.hpp"
#include "TestBase.hpp"
#include "TestClassBase.hpp"

#include <chrono>
#include <mutex>
#include <sstream>
#include <thread>


namespace erbsland::unittest {


Controller::Controller() noexcept
    : _console(new Console()) {
}


Controller::~Controller() {
    delete _console;
}


auto Controller::instance() -> Controller* {
    static std::once_flag storageOnce; // Flag to synchronize controller creation.
    static Controller *storage = nullptr;
    std::call_once(storageOnce, [] {
        storage = new Controller();
    });
    return storage;
}


auto Controller::main(int argc, char **argv) -> int {
    if (auto result = parseCommandLine(argc, argv); result != 0) {
        return result;
    }
    // Sort the test classes by name, as registration may change depending on the compilation order.
    std::stable_sort(
        _testClasses.begin(),
        _testClasses.end(),
        [](const auto &a, const auto &b) -> bool { return a->name() < b->name(); });
    // Create the initial set of tests.
    if (_filter.hasExclusiveSet()) {
        // disable all.
        for (auto &testClass : _testClasses) {
            testClass->setEnabled(false);
        }
        // enable all exclusive tests.
        for (auto &testClass : _testClasses) {
            for (std::size_t i = 0; i < testClass->testCount(); ++i) {
                if (testClass->testMetaData(i).matches(_filter, FilterOption::Exclusive)) {
                    testClass->test(i)->setEnabled(true);
                }
            }
            if (testClass->metaData().matches(_filter, FilterOption::Exclusive)) {
                testClass->setEnabled(true);
            }
        }
    }
    // Now apply all including options.
    for (auto &testClass : _testClasses) {
        for (std::size_t i = 0; i < testClass->testCount(); ++i) {
            if (testClass->testMetaData(i).matches(_filter, FilterOption::Included)) {
                testClass->test(i)->setEnabled(true);
            }
        }
        if (testClass->metaData().matches(_filter, FilterOption::Included)) {
            testClass->setEnabled(true);
        }
    }
    // Finally apply all excluding options.
    for (auto &testClass : _testClasses) {
        for (std::size_t i = 0; i < testClass->testCount(); ++i) {
            if (testClass->testMetaData(i).matches(_filter, FilterOption::Excluded)) {
                testClass->test(i)->setEnabled(false);
            }
        }
        if (testClass->metaData().matches(_filter, FilterOption::Excluded)) {
            testClass->setEnabled(false);
        }
    }
    // Count enabled tests.
    int testCount = 0;
    int testClassCount = 0;
    for (auto &testClass : _testClasses) {
        for (std::size_t i = 0; i < testClass->testCount(); ++i) {
            if (testClass->test(i)->isEnabled()) {
                ++testCount;
            }
        }
        if (testClass->isEnabled()) {
            ++testClassCount;
        }
    }
    std::stringstream text;
    text << "===[ Running " << testClassCount << " test suites with " << testCount << " tests ]===\n";
    auto timeAsString = []() -> std::string {
        using namespace std::chrono;
        std::time_t t = system_clock::to_time_t(system_clock::now());
        std::string str = std::ctime(&t);
        str.resize(str.size()-1);
        return str;
    };
    text << "Start Time: " << timeAsString();
    const auto startTime = std::chrono::steady_clock::now();
    if (!_filter.isEmpty()) {
        text << "\nFilter: " << _filter.toString();
    } else {
        text << "\nFilter: no filter set";
    }
    console()->writeLine(text.str());
    int errors = 0;
    const int totalTaskCount = testClassCount + testCount;
    int currentTask = 1;
    for (auto testClass : _testClasses) {
        text.str({});
        text << "Suite: " << testClass->shortName();
        _currentSuite = testClass->shortName();
        _currentTest = "<ctor>";
        if (testClass->isEnabled()) {
            console()->startTask(text.str(), currentTask, totalTaskCount);
            try {
                testClass->createUnitTest();
            } catch (const std::exception &ex) {
                auto errorCapture = reportError("EXCEPTION!", Console::Red);
                console()->writeLine("Exception while creating the unit test instance.");
                errorCapture->addContextInfo("Exception while creating the unit test instance.");
                auto exceptionType = std::string(typeid(ex).name());
                auto exceptionMessage = std::string(ex.what());
                text.str({});
                text << "Exception Type: " << demangleTypeName(exceptionType) << "\n"
                    << "Exception Message: " << exceptionMessage;
                console()->writeDebug(text.str());
                errorCapture->addDebugInfo(text.str());
                ++errors;
                if (_stopAtFirstError) {
                    break;
                }
                continue;
            } catch(...) {
                auto errorCapture = reportError("EXCEPTION!", Console::Red);
                console()->writeErrorInfo("Unknown exception while creating the unit test instance.");
                errorCapture->addContextInfo("Unknown exception while creating the unit test instance.");
                console()->writeDebug("Unknown exception.");
                ++errors;
                continue;
            }
            if (_waitAfterEachTest) {
                std::this_thread::sleep_for(std::chrono::seconds{1});
            }
            console()->finishTask("OK!", Console::Green);
            ++currentTask;
        } else {
            if (_verbose) {
                console()->startTask(text.str(), currentTask, totalTaskCount);
                console()->finishTask("Skipped", Console::Orange);
            }
            continue;
        }
        for (std::size_t i = 0; i < testClass->testCount(); ++i) {
            auto test = testClass->test(i);
            text.str({});
            if (test->metaData().isPrintMethod()) {
                text << "  Print: " << test->shortName();
            } else {
                text << "  Test: " << test->shortName();
            }
            if (test->isEnabled()) {
                console()->startTask(text.str(), currentTask, totalTaskCount);
            } else {
                if (_verbose) {
                    console()->startTask(text.str(), currentTask, totalTaskCount);
                    console()->finishTask("Skipped", Console::Orange);
                }
                continue;
            }
            _currentTest = test->shortName();
            try {
                if (test->metaData().isPrintMethod()) {
                    _printMethodRunning = true;
                    text.str({});
                    text << "---{ start output from " << testClass->shortName()
                        << " / " << test->shortName() << " }---";
                    console()->writeDebug(text.str());
                }
                testClass->callTest(i);
                if (test->metaData().isPrintMethod()) {
                    _printMethodRunning = false;
                    text.str({});
                    text << "---{ end output from " << testClass->shortName()
                         << " / " << test->shortName() << " }---";
                    console()->writeDebug(text.str());
                }
                if (_waitAfterEachTest) {
                    std::this_thread::sleep_for(std::chrono::seconds{1});
                }
                console()->finishTask("OK!", Console::Green);
            } catch(const AssertFailed&) {
                ++errors;
            } catch (const std::exception &ex) { \
                auto errorCapture = reportError("EXCEPTION!", Console::Red);
                console()->writeErrorInfo("Exception outside of assert clause.");
                errorCapture->addContextInfo("Exception outside of assert clause.");
                auto exceptionType = std::string(typeid(ex).name());
                auto exceptionMessage = std::string(ex.what());
                text.str({});
                text << "Exception Type: " << demangleTypeName(exceptionType) << "\n"
                    << "Exception Message: " << exceptionMessage;
                console()->writeDebug(text.str());
                errorCapture->addDebugInfo(text.str());
                ++errors;
            } catch(...) {
                auto errorCapture = reportError("EXCEPTION!", Console::Red);
                console()->writeErrorInfo("Unknown exception outside of assert clause.");
                errorCapture->addContextInfo("Unknown exception outside of assert clause.");
                console()->writeDebug("Unknown exception.");
                ++errors;
            }
            ++currentTask;
            if (_stopAtFirstError && errors > 0) {
                break;
            }
        }
        if (_stopAtFirstError && errors > 0) {
            break;
        }
    }
    if (errors > 0) {
        if (_showSummary) {
            console()->writeError("===[ ERROR SUMMARY ]===");
            auto it = _capturedErrors.begin();
            for (std::size_t i = 0; it != _capturedErrors.end() && i < 3; ++i, ++it) {
                const auto &errorCapture = *it;
                text.str({});
                text << "Error " << static_cast<int>(i + 1) << " - "
                    << errorCapture->suite() << " / " << errorCapture->test();
                console()->writeErrorTaskLine(
                    text.str(),
                    errorCapture->result(),
                    errorCapture->resultColor());
                const auto &context = errorCapture->contextInfo();
                for (const auto &line : context) {
                    console()->writeErrorInfo(line);
                }
            }
        }
        text.str({});
        text << "===[ ERROR | " << errors << " errors while running the tests. ]===";
        console()->writeError(text.str());
        return 1;
    }
    const auto endTime = std::chrono::steady_clock::now();
    const std::chrono::duration<double> testDuration = endTime - startTime;
    text.str({});
    text << "Total Test Duration: " << std::setprecision(3) << testDuration.count() << " seconds";
    console()->writeLine(text.str());
    console()->writeSuccess("===[ SUCCESS | Successfully run all tests without errors. ]===");
    return 0;
}


void Controller::addTestClass(TestClassBase *testClass) noexcept {
    _testClasses.push_back(testClass);
}


auto Controller::executablePath() -> std::filesystem::path {
    return _executablePath;
}


auto Controller::parseCommandLine(int argc, char **argv) -> int {
    // capture the executable path.
    if (argc > 0) {
        _executablePath = std::filesystem::path(argv[0]);
        try {
            _executablePath = std::filesystem::absolute(_executablePath);
        } catch (const std::filesystem::filesystem_error&) {
            // ignore
        }
    }
    // convert the arguments.
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }
    for (const auto &arg : args) {
        if (arg == "-h" || arg == "-help" || arg == "--help") {
            printHelp();
            return 1;
        }
        if (arg == "-l" || arg == "--list") {
            printList();
            return 1;
        }
        if (arg == "-v" || arg == "--verbose") {
            _verbose = true;
            continue;
        }
        if (arg == "-s") {
            _stopAtFirstError = true;
            continue;
        }
        if (arg == "-c" || arg == "--no-color") {
            console()->setUseColor(false);
            continue;
        }
        if (arg == "-s" || arg == "--no-summary") {
            _showSummary = false;
            continue;
        }
        if (arg == "-Xw") {
            _waitAfterEachTest = true;
            continue;
        }
        auto index = arg.find(':');
        if (index > 0) {
            std::string option = arg.substr(0, index);
            std::string value = arg.substr(index + 1);
            FilterOption type = FilterOption::Exclusive;
            if (option[0] == '+') {
                type = FilterOption::Included;
                option = option.substr(1);
            } else if (option[0] == '-') {
                type = FilterOption::Excluded;
                option = option.substr(1);
            }
            FilterRule *rule = nullptr;
            if (option == "name") {
                rule = &_filter.names;
            } else if (option == "target") {
                rule = &_filter.targets;
            } else if (option == "tag") {
                rule = &_filter.tags;
            } else {
                std::stringstream text;
                text << "Unknown command line argument \"" << arg << "\"\n\n";
                console()->writeError(text.str());
                printHelp();
                return 1;
            }
            switch (type) {
                case FilterOption::Exclusive:
                    rule->exclusive.insert(value);
                    break;
                case FilterOption::Included:
                    rule->included.insert(value);
                    break;
                case FilterOption::Excluded:
                    rule->excluded.insert(value);
                    break;
            }
        } else {
            std::stringstream text;
            text << "Unknown command line argument \"" << arg << "\"\n\n";
            console()->writeError(text.str());
            printHelp();
            return 1;
        }
    }
    return 0;
}

void Controller::printHelp() {
    std::stringstream text;
    text << "Erbsland Unit Test Help:\n"
        << "  -h/--help ......... Display this help\n"
        << "  -v/--verbose ...... Display verbose messages. Skipped tests.\n"
        << "  -e ................ Stop at the first error.\n"
        << "  -l/--list ......... List all suites and tests. Do not run any test.\n"
        << "  -c/--no-color ..... Do not colorize the output and disable status updates.\n"
        << "  -s/--no-summary ... Do not list the first three errors at the end of the run.\n"
        << "  name:<name> ....... Exclusively run tests with the specified test or class name (case sensitive).\n"
        << "  +name:<name> ...... Run tests with the specified test or class name, even optional ones.\n"
        << "  -name:<name> ...... Skip tests with the specified test or class name.\n"
        << "  target:<target> ... Exclusively run tests for the specified target.\n"
        << "  +target:<target> .. Run tests with for the specified target, even optional ones.\n"
        << "  -target:<target> .. Skip tests with for the specified target.\n"
        << "  tag:<tag> ......... Exclusively run tests with the specified tags.\n"
        << "  +tag:<tag> ........ Run tests with the specified tags, even optional ones.\n"
        << "  -tag:<tag> ........ Skip tests with the specified tags.\n"
        << "\n"
        << "By default all tests that are not marked with `SKIP_BY_DEFAULT()` are enabled.\n"
        << "You can individually add `+` or remove `-` tests from this initial set.\n"
        << "\n"
        << "If you specify one or more options like `<opt>:<tag>`, only tests with \n"
        << "are enabled, and further `+/-` options will change this set.\n"
        << "\n"
        << "The processing order of the options is <opt>, +<opt>, -<opt> and does not depend\n"
        << "on the order how they are specified on the command line. Therefore `-` always have\n"
        << "the highest priority and will skip these tests no mather what was specified otherwise.\n";
    console()->writeLine(text.str());
}


void Controller::printList() {
    console()->writeLine("===[ List all test suites and tests ]===");
    for (auto testClass : _testClasses) {
        console()->writeTestEntry("Suite", testClass->metaData());
        for (std::size_t i = 0; i < testClass->testCount(); ++i) {
            console()->writeTestEntry("  Test", testClass->testMetaData(i));
        }
    }
    console()->writeSuccess("Done!");
}


auto Controller::console() const noexcept -> Console * {
    return _console;
}


void Controller::writeFromUnitTest(const std::string &text) {
    if (_printMethodRunning) {
        console()->writeLine(text);
    } else {
        console()->writeDebug(text);
    }
}


auto Controller::reportError(const std::string &result, Console::Color textColor) -> ErrorCapturePtr {
    auto errorCapture = std::make_shared<ErrorCapture>(_currentSuite, _currentTest, result, textColor);
    _capturedErrors.push_back(errorCapture);
    console()->finishTask(result, textColor);
    return  errorCapture;
}


}

