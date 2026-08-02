// Copyright (c) 2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "Console.hpp"
#include "ErrorCapture.hpp"
#include "Timing.hpp"

#include <exception>
#include <list>
#include <string>
#include <vector>

namespace erbsland::unittest {

struct Filter;
class TestClassBase;

/// @internal
/// Render all high-level output of the unit-test runner.
/// @tested{OutputTest}
class Output final {
public:
    /// Enable or disable colored output.
    void setUseColor(bool enabled);
    /// Access the low-level console.
    [[nodiscard]] auto console() noexcept -> Console *;
    /// Write a regular line.
    void writeLine(const std::string &text);
    /// Write an error line.
    void writeError(const std::string &text);
    /// Write an error detail line.
    void writeErrorInfo(const std::string &text);
    /// Write a debug line.
    void writeDebug(const std::string &text);
    /// Reset terminal formatting.
    void resetFormatting();

public: // run display
    /// Display the test-run header.
    void writeRunHeader(int suiteCount, int testCount, const Filter &filter);
    /// Display all registered test suites and methods.
    void writeList(const std::vector<TestClassBase *> &testClasses);
    /// Start a task status line.
    void startTask(const std::string &text, int taskNumber, int totalTasks);
    /// Finish a task status line.
    void finishTask(const std::string &result, ConsoleColor textColor, bool showDuration, TestDuration duration = {});
    /// Display a skipped task.
    void writeSkippedTask(const std::string &text, int taskNumber, int totalTasks);
    /// Begin the delimited output of a print method.
    void beginPrintMethod(const std::string &suite, const std::string &test);
    /// End the delimited output of a print method.
    void endPrintMethod(const std::string &suite, const std::string &test);
    /// Stop treating regular test output as print-method output.
    void cancelPrintMethod() noexcept;
    /// Write text emitted from a unit test.
    void writeFromUnitTest(const std::string &text);

public: // errors
    /// Capture a runner error.
    /// @param suite The active suite name.
    /// @param test The active method name.
    /// @param result The displayed task result.
    /// @param textColor The result color.
    /// @return The new error capture.
    [[nodiscard]] auto captureError(
        const std::string &suite, const std::string &test, const std::string &result, ConsoleColor textColor)
        -> ErrorCapturePtr;
    /// Display and capture a standard exception.
    void writeException(
        const ErrorCapturePtr &errorCapture, const std::string &context, const std::exception &exception);
    /// Display and capture an unknown exception.
    void writeUnknownException(const ErrorCapturePtr &errorCapture, const std::string &context);
    /// Display the configured error summary.
    void writeErrorSummary();
    /// Display the final error banner.
    void writeErrorBanner(int errorCount);

public: // timing and success
    /// Display a labeled timing value.
    void writeTimingValue(const std::string &label, TestDuration duration);
    /// Display all timing statistics.
    void writeTimingStatistics(const TimingStatistics &statistics);
    /// Display the successful-run duration in the legacy format.
    void writeLegacyTotalDuration(TestDuration duration);
    /// Display the final success banner.
    void writeSuccessBanner();

private:
    /// Display one timing statistics group.
    void writeTimingSummary(const std::string &title, const TimingSummary &summary);

private:
    Console _console{};                         ///< The low-level console interface.
    std::list<ErrorCapturePtr> _capturedErrors; ///< Errors captured during the run.
    bool _printMethodRunning{false};            ///< True while a print method emits output.
};

}
