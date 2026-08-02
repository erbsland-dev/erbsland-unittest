// Copyright (c) 2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#include "Output.hpp"

#include "Demangle.hpp"
#include "Filter.hpp"
#include "TestClassBase.hpp"

#include <chrono>
#include <format>
#include <iomanip>
#include <memory>
#include <sstream>
#include <typeinfo>

namespace erbsland::unittest {

void Output::setUseColor(const bool enabled) {
    _console.setUseColor(enabled);
}

auto Output::console() noexcept -> Console * {
    return &_console;
}

void Output::writeLine(const std::string &text) {
    _console.writeLine(text);
}

void Output::writeError(const std::string &text) {
    _console.writeError(text);
}

void Output::writeErrorInfo(const std::string &text) {
    _console.writeErrorInfo(text);
}

void Output::writeDebug(const std::string &text) {
    _console.writeDebug(text);
}

void Output::resetFormatting() {
    _console.resetFormatting();
}

void Output::writeRunHeader(const int suiteCount, const int testCount, const Filter &filter) {
    auto text = std::stringstream{};
    text << "===[ Running " << suiteCount << " test suites with " << testCount << " tests ]===\n";
    const auto now = std::chrono::system_clock::now();
    text << "Start Time: " << std::format("{:%c}", now);
    if (!filter.isEmpty()) {
        text << "\nFilter: " << filter.toString();
    } else {
        text << "\nFilter: no filter set";
    }
    writeLine(text.str());
}

void Output::writeList(const std::vector<TestClassBase *> &testClasses) {
    writeLine("===[ List all test suites and tests ]===");
    for (const auto testClass : testClasses) {
        _console.writeTestEntry("Suite", testClass->metaData());
        for (std::size_t index = 0; index < testClass->testCount(); ++index) {
            _console.writeTestEntry("  Test", testClass->testMetaData(index));
        }
    }
    _console.writeSuccess("Done!");
}

void Output::startTask(const std::string &text, const int taskNumber, const int totalTasks) {
    _console.startTask(text, taskNumber, totalTasks);
}

void Output::finishTask(
    const std::string &result, const ConsoleColor textColor, const bool showDuration, const TestDuration duration) {
    const auto formattedDuration = showDuration ? formatTestDuration(duration) : std::string{};
    _console.finishTask(result, textColor, formattedDuration);
}

void Output::writeSkippedTask(const std::string &text, const int taskNumber, const int totalTasks) {
    startTask(text, taskNumber, totalTasks);
    finishTask("Skipped", ConsoleColor::Orange, false);
}

void Output::beginPrintMethod(const std::string &suite, const std::string &test) {
    _printMethodRunning = true;
    writeDebug("---{ start output from " + suite + " / " + test + " }---");
}

void Output::endPrintMethod(const std::string &suite, const std::string &test) {
    _printMethodRunning = false;
    writeDebug("---{ end output from " + suite + " / " + test + " }---");
}

void Output::cancelPrintMethod() noexcept {
    _printMethodRunning = false;
}

void Output::writeFromUnitTest(const std::string &text) {
    if (_printMethodRunning) {
        writeLine(text);
    } else {
        writeDebug(text);
    }
}

auto Output::captureError(
    const std::string &suite, const std::string &test, const std::string &result, const ConsoleColor textColor)
    -> ErrorCapturePtr {
    auto errorCapture = std::make_shared<ErrorCapture>(suite, test, result, textColor);
    _capturedErrors.push_back(errorCapture);
    return errorCapture;
}

void Output::writeException(
    const ErrorCapturePtr &errorCapture, const std::string &context, const std::exception &exception) {
    writeErrorInfo(context);
    errorCapture->addContextInfo(context);
    const auto exceptionType = std::string{typeid(exception).name()};
    auto text = std::stringstream{};
    text << "Exception Type: " << demangleTypeName(exceptionType) << "\n"
         << "Exception Message: " << exception.what();
    writeDebug(text.str());
    errorCapture->addDebugInfo(text.str());
}

void Output::writeUnknownException(const ErrorCapturePtr &errorCapture, const std::string &context) {
    writeErrorInfo(context);
    errorCapture->addContextInfo(context);
    writeDebug("Unknown exception.");
}

void Output::writeErrorSummary() {
    writeError("===[ ERROR SUMMARY ]===");
    auto iterator = _capturedErrors.begin();
    for (std::size_t index = 0; iterator != _capturedErrors.end() && index < 3; ++index, ++iterator) {
        const auto &errorCapture = *iterator;
        const auto task = std::format("Error {} - {} / {}", index + 1, errorCapture->suite(), errorCapture->test());
        _console.writeErrorTaskLine(task, errorCapture->result(), errorCapture->resultColor());
        for (const auto &line : errorCapture->contextInfo()) {
            writeErrorInfo(line);
        }
    }
}

void Output::writeErrorBanner(const int errorCount) {
    writeError(std::format("===[ ERROR | {} errors while running the tests. ]===", errorCount));
}

void Output::writeTimingValue(const std::string &label, const TestDuration duration) {
    auto line = ConsoleLine{};
    line.addText(label);
    line.addText(formatTestDuration(duration), ConsoleColor::Cyan);
    _console.writeLine(line);
}

void Output::writeTimingStatistics(const TimingStatistics &statistics) {
    auto heading = ConsoleLine{};
    heading.addText("===[ TIME STATISTICS ]===", ConsoleColor::Cyan);
    _console.writeLine(heading);
    writeTimingSummary("Tests", statistics.tests());
    writeTimingSummary("Suites", statistics.suites());
}

void Output::writeTimingSummary(const std::string &title, const TimingSummary &summary) {
    auto heading = ConsoleLine{};
    heading.addText(title, ConsoleColor::Cyan);
    _console.writeLine(heading);
    if (summary.count == 0) {
        auto line = ConsoleLine{};
        line.addText(
            "  No " + std::string{title == "Tests" ? "tests" : "suites"} + " measured.", ConsoleColor::DarkCyan);
        _console.writeLine(line);
        return;
    }

    auto values = ConsoleLine{};
    values.addText("  Total: ", ConsoleColor::DarkCyan);
    values.addText(formatTestDuration(summary.total), ConsoleColor::Cyan);
    values.addText(" | Average: ", ConsoleColor::DarkCyan);
    values.addText(formatTestDuration(summary.average), ConsoleColor::Cyan);
    values.addText(" | Median: ", ConsoleColor::DarkCyan);
    values.addText(formatTestDuration(summary.median), ConsoleColor::Cyan);
    _console.writeLine(values);

    auto slowestHeading = ConsoleLine{};
    slowestHeading.addText("  Slowest " + title + ":", ConsoleColor::Cyan);
    _console.writeLine(slowestHeading);
    for (std::size_t index = 0; index < summary.slowest.size(); ++index) {
        const auto &record = summary.slowest[index];
        auto line = ConsoleLine{};
        line.addText(std::format("  {:>2}. ", index + 1), ConsoleColor::DarkCyan);
        line.addText(record.suite, ConsoleColor::White);
        if (!record.test.empty()) {
            line.addText(" / ", ConsoleColor::DarkCyan);
            line.addText(record.test, ConsoleColor::White);
        }
        if (record.interrupted) {
            line.addText(" [interrupted]", ConsoleColor::DarkCyan);
        }
        line.addText(" - ", ConsoleColor::DarkCyan);
        line.addText(formatTestDuration(record.duration), ConsoleColor::Cyan);
        _console.writeLine(line);
    }
}

void Output::writeLegacyTotalDuration(const TestDuration duration) {
    auto text = std::stringstream{};
    text << "Total Test Duration: " << std::setprecision(3) << duration.count() << " seconds";
    writeLine(text.str());
}

void Output::writeSuccessBanner() {
    _console.writeSuccess("===[ SUCCESS | Successfully run all tests without errors. ]===");
}

}
