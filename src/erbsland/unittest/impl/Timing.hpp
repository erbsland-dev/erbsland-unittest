// Copyright (c) 2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <chrono>
#include <cstddef>
#include <string>
#include <vector>

namespace erbsland::unittest {

/// @internal
/// The duration type used for test timing.
using TestDuration = std::chrono::duration<double>;

/// @internal
/// The kind of a measured runner entry.
/// @tested{TimingTest}
enum class TimingKind {
    Test,  ///< A regular test method.
    Print, ///< A print method.
    Suite, ///< A complete test suite.
};

/// @internal
/// A single measured runner entry.
/// @tested{TimingTest}
struct TimingRecord {
    std::string suite;       ///< The short suite name.
    std::string test;        ///< The short test name, or empty for suites.
    TimingKind kind{};       ///< The kind of measured entry.
    TestDuration duration{}; ///< The measured duration.
    bool interrupted{};      ///< If execution was interrupted by an error or early stop.
};

/// @internal
/// Aggregated timing values and the slowest entries.
/// @tested{TimingTest}
struct TimingSummary {
    std::size_t count{};               ///< The number of summarized entries.
    TestDuration total{};              ///< The sum of all durations.
    TestDuration average{};            ///< The arithmetic average duration.
    TestDuration median{};             ///< The median duration.
    std::vector<TimingRecord> slowest; ///< Up to ten slowest entries.
};

/// @internal
/// Collect and summarize runner timings.
/// @tested{TimingTest}
class TimingStatistics final {
public:
    /// Add a measured test or print method.
    /// @param suite The short suite name.
    /// @param test The short method name.
    /// @param kind The method kind.
    /// @param duration The measured duration.
    /// @param interrupted If execution was interrupted.
    void addMethod(
        const std::string &suite, const std::string &test, TimingKind kind, TestDuration duration, bool interrupted);
    /// Add a measured suite.
    /// @param suite The short suite name.
    /// @param duration The measured duration.
    /// @param interrupted If execution was interrupted.
    void addSuite(const std::string &suite, TestDuration duration, bool interrupted);
    /// Summarize regular test methods, excluding print methods.
    [[nodiscard]] auto tests() const -> TimingSummary;
    /// Summarize suites.
    [[nodiscard]] auto suites() const -> TimingSummary;

private:
    /// Summarize selected records.
    /// @param records The records to summarize.
    /// @return The calculated summary.
    [[nodiscard]] static auto summarize(std::vector<TimingRecord> records) -> TimingSummary;

private:
    std::vector<TimingRecord> _methods; ///< All measured test and print methods.
    std::vector<TimingRecord> _suites;  ///< All measured suites.
};

/// Format a duration using an adaptive unit and three significant digits.
/// @param duration The duration to format.
/// @return The formatted duration.
/// @tested{TimingTest}
[[nodiscard]] auto formatTestDuration(TestDuration duration) -> std::string;

}
