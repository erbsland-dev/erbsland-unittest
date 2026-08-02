// Copyright (c) 2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0

#include <erbsland/unittest/impl/Output.hpp>
#include <erbsland/unittest/UnitTest.hpp>

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

namespace {

using erbsland::unittest::Output;
using erbsland::unittest::TestDuration;
using erbsland::unittest::TimingKind;
using erbsland::unittest::TimingStatistics;
using namespace std::chrono_literals;

/// Capture standard output for one scope.
/// @tested{OutputTest}
class StandardOutputCapture final {
public:
    /// Start capturing standard output.
    StandardOutputCapture() : _previous{std::cout.rdbuf(_buffer.rdbuf())} {}
    /// Restore standard output.
    ~StandardOutputCapture() { std::cout.rdbuf(_previous); }

public:
    /// Access the captured text.
    [[nodiscard]] auto text() const -> std::string { return _buffer.str(); }

private:
    std::ostringstream _buffer{}; ///< The captured output buffer.
    std::streambuf *_previous;    ///< The previous standard-output buffer.
};

}

TESTED_TARGETS(Output Console)
class OutputTest final : public el::UnitTest {
public:
    void testTaskTimingWithoutColor() {
        auto actual = std::string{};
        {
            auto capture = StandardOutputCapture{};
            auto output = Output{};
            output.setUseColor(false);
            output.startTask("  Test: Example", 1, 1);
            output.finishTask("OK!", erbsland::unittest::ConsoleColor::Green, true, 230ms);
            actual = capture.text();
        }
        REQUIRE_EQUAL(actual, "-   Test: Example (230ms) OK!\n");
    }

    void testOnlyDurationTokenIsCyan() {
        auto actual = std::string{};
        {
            auto capture = StandardOutputCapture{};
            auto output = Output{};
            output.startTask("  Test: Example", 1, 1);
            output.finishTask("OK!", erbsland::unittest::ConsoleColor::Green, true, 230ms);
            actual = capture.text();
        }
        REQUIRE(actual.find("(\x1b[96m230ms\x1b[39m) ") != std::string::npos);
    }

    void testStatisticsLayout() {
        auto statistics = TimingStatistics{};
        statistics.addMethod("Suite", "Fast", TimingKind::Test, TestDuration{1.0}, false);
        statistics.addMethod("Suite", "Slow", TimingKind::Test, TestDuration{3.0}, true);
        statistics.addSuite("Suite", TestDuration{5.0}, true);

        auto actual = std::string{};
        {
            auto capture = StandardOutputCapture{};
            auto output = Output{};
            output.setUseColor(false);
            output.writeTimingStatistics(statistics);
            actual = capture.text();
        }
        REQUIRE(actual.find("===[ TIME STATISTICS ]===") != std::string::npos);
        REQUIRE(actual.find("Total: 4.00s | Average: 2.00s | Median: 2.00s") != std::string::npos);
        REQUIRE(actual.find("Suite / Slow [interrupted] - 3.00s") != std::string::npos);
        REQUIRE(actual.find("Slowest Suites:") != std::string::npos);
    }

    void testEmptyStatisticsLayout() {
        auto actual = std::string{};
        {
            auto capture = StandardOutputCapture{};
            auto output = Output{};
            output.setUseColor(false);
            output.writeTimingStatistics(TimingStatistics{});
            actual = capture.text();
        }
        REQUIRE(actual.find("No tests measured.") != std::string::npos);
        REQUIRE(actual.find("No suites measured.") != std::string::npos);
    }
};
