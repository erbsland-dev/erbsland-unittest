// Copyright (c) 2023-2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#include "Controller.hpp"

#include "AssertFailed.hpp"
#include "TestBase.hpp"
#include "TestClassBase.hpp"

#include <algorithm>
#include <chrono>
#include <exception>
#include <mutex>
#include <ranges>
#include <thread>

namespace erbsland::unittest {

auto Controller::instance() -> Controller * {
    static auto storageOnce = std::once_flag{};
    static Controller *storage = nullptr;
    std::call_once(storageOnce, []() -> void { storage = new Controller(); });
    return storage;
}

auto Controller::main(const int argc, char **argv) -> int {
    const auto commandLineResult = _commandLine.parse(argc, argv, _output);
    if (commandLineResult.action == CommandLine::Action::Exit) {
        return commandLineResult.exitCode;
    }
    if (commandLineResult.action == CommandLine::Action::List) {
        _output.writeList(_testClasses);
        return commandLineResult.exitCode;
    }

    _output.resetFormatting();
    sortTestClasses();
    applyFilter();
    return runTests(countEnabledTests());
}

void Controller::addTestClass(TestClassBase *testClass) noexcept {
    _testClasses.push_back(testClass);
}

auto Controller::executablePath() -> std::filesystem::path {
    return _commandLine.settings().executablePath;
}

void Controller::writeFromUnitTest(const std::string &text) {
    _output.writeFromUnitTest(text);
}

auto Controller::console() noexcept -> Console * {
    return _output.console();
}

void Controller::sortTestClasses() {
    std::ranges::stable_sort(
        _testClasses, [](const auto &left, const auto &right) -> bool { return left->name() < right->name(); });
}

void Controller::applyFilter() {
    const auto &filter = _commandLine.settings().filter;
    if (filter.hasExclusiveSet()) {
        for (auto &testClass : _testClasses) {
            testClass->setEnabled(false);
        }
        for (auto &testClass : _testClasses) {
            for (std::size_t index = 0; index < testClass->testCount(); ++index) {
                if (testClass->testMetaData(index).matches(filter, FilterOption::Exclusive)) {
                    testClass->test(index)->setEnabled(true);
                }
            }
            if (testClass->metaData().matches(filter, FilterOption::Exclusive)) {
                testClass->setEnabled(true);
            }
        }
    }

    for (auto &testClass : _testClasses) {
        for (std::size_t index = 0; index < testClass->testCount(); ++index) {
            if (testClass->testMetaData(index).matches(filter, FilterOption::Included)) {
                testClass->test(index)->setEnabled(true);
            }
        }
        if (testClass->metaData().matches(filter, FilterOption::Included)) {
            testClass->setEnabled(true);
        }
    }

    for (auto &testClass : _testClasses) {
        for (std::size_t index = 0; index < testClass->testCount(); ++index) {
            if (testClass->testMetaData(index).matches(filter, FilterOption::Excluded)) {
                testClass->test(index)->setEnabled(false);
            }
        }
        if (testClass->metaData().matches(filter, FilterOption::Excluded)) {
            testClass->setEnabled(false);
        }
    }
}

auto Controller::countEnabledTests() const -> TestCounts {
    auto result = TestCounts{};
    for (const auto testClass : _testClasses) {
        for (std::size_t index = 0; index < testClass->testCount(); ++index) {
            if (testClass->test(index)->isEnabled()) {
                ++result.tests;
            }
        }
        if (testClass->isEnabled()) {
            ++result.suites;
        }
    }
    return result;
}

auto Controller::runTests(const TestCounts &counts) -> int {
    _runState = {};
    _runState.currentTask = 1;
    _runState.totalTasks = counts.suites + counts.tests;
    _timingStatistics = {};
    const auto &settings = _commandLine.settings();
    _output.writeRunHeader(counts.suites, counts.tests, settings.filter);
    const auto startTime = std::chrono::steady_clock::now();

    for (const auto testClass : _testClasses) {
        const auto interrupted = runSuite(testClass);
        if (interrupted && settings.stopAtFirstError) {
            break;
        }
    }
    return finishRun(std::chrono::steady_clock::now() - startTime);
}

auto Controller::runSuite(TestClassBase *testClass) -> bool {
    const auto suiteStart = std::chrono::steady_clock::now();
    const auto suiteName = testClass->shortName();
    const auto suiteTask = "Suite: " + suiteName;
    const auto &settings = _commandLine.settings();
    if (!testClass->isEnabled()) {
        if (settings.verbose) {
            _output.writeSkippedTask(suiteTask, _runState.currentTask, _runState.totalTasks);
        }
        return false;
    }

    _output.startTask(suiteTask, _runState.currentTask, _runState.totalTasks);
    beginTask(suiteName, "<ctor>", false);
    auto interrupted = !constructSuite(testClass);
    ++_runState.currentTask;
    for (std::size_t index = 0; !interrupted && index < testClass->testCount(); ++index) {
        runMethod(testClass, index);
        if (settings.stopAtFirstError && _runState.errors > 0) {
            interrupted = true;
        }
    }

    const TestDuration suiteDuration = std::chrono::steady_clock::now() - suiteStart;
    if (isTimingEnabled()) {
        _timingStatistics.addSuite(suiteName, suiteDuration, interrupted);
    }
    if (settings.showTestTime) {
        _output.writeTimingValue("  Total Time: ", suiteDuration);
    }
    return interrupted;
}

auto Controller::constructSuite(TestClassBase *testClass) -> bool {
    try {
        testClass->createUnitTest();
        waitAfterTask();
        finishCurrentTask("Running", ConsoleColor::White);
        return true;
    } catch (const std::exception &exception) {
        const auto errorCapture = reportError("EXCEPTION!", ConsoleColor::Red);
        _output.writeException(errorCapture, "Exception while creating the unit test instance.", exception);
    } catch (...) {
        const auto errorCapture = reportError("EXCEPTION!", ConsoleColor::Red);
        _output.writeUnknownException(errorCapture, "Unknown exception while creating the unit test instance.");
    }
    finishCurrentTask(_runState.task.errorResult, _runState.task.errorColor);
    ++_runState.errors;
    return false;
}

void Controller::runMethod(TestClassBase *testClass, const std::size_t index) {
    const auto test = testClass->test(index);
    const auto isPrintMethod = test->metaData().isPrintMethod();
    const auto task = std::string{isPrintMethod ? "  Print: " : "  Test: "} + test->shortName();
    const auto &settings = _commandLine.settings();
    if (!test->isEnabled()) {
        if (settings.verbose) {
            _output.writeSkippedTask(task, _runState.currentTask, _runState.totalTasks);
        }
        return;
    }

    _output.startTask(task, _runState.currentTask, _runState.totalTasks);
    beginTask(testClass->shortName(), test->shortName(), true);
    auto interrupted = false;
    auto duration = TestDuration{};
    try {
        if (isPrintMethod) {
            _output.beginPrintMethod(_runState.task.suite, _runState.task.test);
        }
        _runState.task.start = std::chrono::steady_clock::now();
        testClass->callTest(index);
        duration = currentTaskDuration();
        if (isPrintMethod) {
            _output.endPrintMethod(_runState.task.suite, _runState.task.test);
        }
        waitAfterTask();
        if (!_runState.task.hadError) {
            finishCurrentTask("OK!", ConsoleColor::Green, duration);
        }
    } catch (const AssertFailed &) {
        duration = currentTaskDuration();
        interrupted = true;
    } catch (const std::exception &exception) {
        duration = currentTaskDuration();
        interrupted = true;
        const auto errorCapture = reportError("EXCEPTION!", ConsoleColor::Red);
        _output.writeException(errorCapture, "Exception outside of assert clause.", exception);
    } catch (...) {
        duration = currentTaskDuration();
        interrupted = true;
        const auto errorCapture = reportError("EXCEPTION!", ConsoleColor::Red);
        _output.writeUnknownException(errorCapture, "Unknown exception outside of assert clause.");
    }

    _output.cancelPrintMethod();
    if (_runState.task.hadError) {
        finishCurrentTask(_runState.task.errorResult, _runState.task.errorColor, duration);
    }
    _runState.task.timed = false;
    if (_runState.task.hadError) {
        ++_runState.errors;
    }
    if (isTimingEnabled()) {
        const auto kind = isPrintMethod ? TimingKind::Print : TimingKind::Test;
        _timingStatistics.addMethod(_runState.task.suite, _runState.task.test, kind, duration, interrupted);
    }
    ++_runState.currentTask;
}

auto Controller::finishRun(const TestDuration totalDuration) -> int {
    const auto &settings = _commandLine.settings();
    if (_runState.errors > 0) {
        if (settings.showSummary) {
            _output.writeErrorSummary();
        }
        _output.writeErrorBanner(_runState.errors);
        if (settings.showTimeStatistics) {
            _output.writeTimingStatistics(_timingStatistics);
        }
        _output.resetFormatting();
        return 1;
    }

    if (isTimingEnabled()) {
        _output.writeTimingValue("Total Test Duration: ", totalDuration);
    } else {
        _output.writeLegacyTotalDuration(totalDuration);
    }
    _output.writeSuccessBanner();
    if (settings.showTimeStatistics) {
        _output.writeTimingStatistics(_timingStatistics);
    }
    _output.resetFormatting();
    return 0;
}

void Controller::beginTask(const std::string &suite, const std::string &test, const bool timed) {
    _runState.task = {};
    _runState.task.suite = suite;
    _runState.task.test = test;
    _runState.task.timed = timed;
}

auto Controller::isTimingEnabled() const noexcept -> bool {
    const auto &settings = _commandLine.settings();
    return settings.showTestTime || settings.showTimeStatistics;
}

auto Controller::currentTaskDuration() const -> TestDuration {
    if (!_runState.task.timed) {
        return {};
    }
    return std::chrono::steady_clock::now() - _runState.task.start;
}

void Controller::finishCurrentTask(
    const std::string &result, const ConsoleColor textColor, const std::optional<TestDuration> duration) {
    if (_runState.task.finished) {
        return;
    }
    const auto showDuration = _commandLine.settings().showTestTime && _runState.task.timed;
    _output.finishTask(result, textColor, showDuration, duration.value_or(currentTaskDuration()));
    _runState.task.finished = true;
}

void Controller::waitAfterTask() const {
    if (_commandLine.settings().waitAfterEachTest) {
        std::this_thread::sleep_for(std::chrono::seconds{1});
    }
}

auto Controller::reportError(const std::string &result, const ConsoleColor textColor) -> ErrorCapturePtr {
    auto errorCapture = _output.captureError(_runState.task.suite, _runState.task.test, result, textColor);
    if (!_runState.task.hadError) {
        _runState.task.errorResult = result;
        _runState.task.errorColor = textColor;
    }
    _runState.task.hadError = true;
    return errorCapture;
}

}
