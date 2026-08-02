// Copyright (c) 2023-2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "CommandLine.hpp"
#include "ErrorCapture.hpp"
#include "Output.hpp"
#include "Timing.hpp"

#include <chrono>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace erbsland::unittest {

class TestClassBase;

/// @internal
/// Coordinate registration and execution of all unit tests.
/// @tested{RunnerFailureTest}
class Controller {
private:
    /// Counts of enabled suites and methods.
    /// @tested{RunnerFailureTest}
    struct TestCounts {
        int suites{}; ///< The number of enabled suites.
        int tests{};  ///< The number of enabled methods.
    };

    /// State of the currently active task.
    /// @tested{RunnerFailureTest}
    struct TaskState {
        std::string suite;                           ///< The active suite name.
        std::string test;                            ///< The active method name.
        std::string errorResult;                     ///< The first reported error status.
        ConsoleColor errorColor{};                   ///< The color of the first error status.
        std::chrono::steady_clock::time_point start; ///< The method start time.
        bool timed{false};                           ///< True if the task has an active timer.
        bool finished{false};                        ///< True after its status was written.
        bool hadError{false};                        ///< True after an error was reported.
    };

    /// State of the current complete test run.
    /// @tested{RunnerFailureTest}
    struct RunState {
        int errors{};       ///< The number of failed methods and constructors.
        int currentTask{1}; ///< The number of the current task.
        int totalTasks{};   ///< The total number of enabled tasks.
        TaskState task{};   ///< The active task state.
    };

public:
    /// Construct an empty controller.
    Controller() noexcept = default;

public:
    /// Run all registered unit tests.
    auto main(int argc, char *argv[]) -> int;
    /// Add an instance of a new unit test to the global list.
    /// Use the macro `ADD_CLASS(Example)` after your unit test class to register it.
    /// @param testClass The test class to add.
    void addTestClass(TestClassBase *testClass) noexcept;
    /// Access the executable path.
    auto executablePath() -> std::filesystem::path;
    /// Write messages from the unittest.
    void writeFromUnitTest(const std::string &text);
    /// Report an error.
    auto reportError(const std::string &result, ConsoleColor textColor) -> ErrorCapturePtr;
    /// Access the console.
    [[nodiscard]] auto console() noexcept -> Console *;

public:
    /// Access the global controller.
    [[nodiscard]] static auto instance() -> Controller *;

private: // preparation
    /// Sort all registered suites into deterministic order.
    void sortTestClasses();
    /// Apply the configured filter to all tests.
    void applyFilter();
    /// Count all enabled suites and methods.
    [[nodiscard]] auto countEnabledTests() const -> TestCounts;

private: // execution
    /// Run the prepared test selection.
    [[nodiscard]] auto runTests(const TestCounts &counts) -> int;
    /// Run one test suite.
    /// @param testClass The suite to execute.
    /// @return True if execution of the suite was interrupted.
    [[nodiscard]] auto runSuite(TestClassBase *testClass) -> bool;
    /// Construct a test suite instance.
    /// @param testClass The suite to construct.
    /// @return True if construction was successful.
    [[nodiscard]] auto constructSuite(TestClassBase *testClass) -> bool;
    /// Run one test or print method.
    void runMethod(TestClassBase *testClass, std::size_t index);
    /// Finish the complete run and return its exit code.
    [[nodiscard]] auto finishRun(TestDuration totalDuration) -> int;

private: // task state
    /// Start tracking a new task.
    void beginTask(const std::string &suite, const std::string &test, bool timed);
    /// Test if any timing mode is active.
    [[nodiscard]] auto isTimingEnabled() const noexcept -> bool;
    /// Get the elapsed duration for the current task.
    [[nodiscard]] auto currentTaskDuration() const -> TestDuration;
    /// Finish the current task once.
    void finishCurrentTask(
        const std::string &result, ConsoleColor textColor, std::optional<TestDuration> duration = std::nullopt);
    /// Wait after a successfully completed task if requested.
    void waitAfterTask() const;

private:
    CommandLine _commandLine{};                  ///< Parsed command-line settings.
    Output _output{};                            ///< High-level runner output.
    std::vector<TestClassBase *> _testClasses{}; ///< All registered test classes.
    RunState _runState{};                        ///< State of the active test run.
    TimingStatistics _timingStatistics{};        ///< Collected method and suite timings.
};

}
