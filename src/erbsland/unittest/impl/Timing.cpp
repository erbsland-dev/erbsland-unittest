// Copyright (c) 2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#include "Timing.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <format>
#include <iterator>
#include <numeric>
#include <ranges>
#include <string_view>
#include <utility>

namespace erbsland::unittest {

namespace {

constexpr std::size_t cSlowestCount = 10; ///< The maximum number of slow entries in a report.

/// Conversion information for a displayed time unit.
/// @tested{TimingTest}
struct TimeUnit {
    double seconds;          ///< The number of seconds represented by one unit.
    double limit;            ///< The value at which the next unit is selected, or zero for the last unit.
    std::string_view suffix; ///< The displayed unit suffix.
};

/// The supported time units in ascending order.
constexpr auto cTimeUnits = std::array{
    TimeUnit{0.001, 1000.0, "ms"},
    TimeUnit{1.0, 60.0, "s"},
    TimeUnit{60.0, 60.0, "m"},
    TimeUnit{3600.0, 0.0, "h"},
};

/// Round a value to three significant digits.
/// @param value The value to round.
/// @return The rounded value.
/// @tested{TimingTest}
auto roundSignificant(const double value) -> double {
    if (value == 0.0) {
        return 0.0;
    }
    const auto exponent = std::floor(std::log10(std::abs(value)));
    const auto factor = std::pow(10.0, 2.0 - exponent);
    return std::round(value * factor) / factor;
}

/// Format a value with three significant digits and no exponent.
/// @param value The already rounded value.
/// @return The formatted value.
/// @tested{TimingTest}
auto formatSignificant(const double value) -> std::string {
    if (value == 0.0) {
        return "0";
    }
    const auto exponent = static_cast<int>(std::floor(std::log10(std::abs(value))));
    const auto precision = std::max(0, 2 - exponent);
    switch (precision) {
    case 0:
        return std::format("{:.0f}", value);
    case 1:
        return std::format("{:.1f}", value);
    case 2:
        return std::format("{:.2f}", value);
    case 3:
        return std::format("{:.3f}", value);
    case 4:
        return std::format("{:.4f}", value);
    default:
        return std::format("{:.5f}", value);
    }
}

/// Compare timing records for the slowest-entry order.
/// @param left The left record.
/// @param right The right record.
/// @return True if the left record shall be ordered first.
/// @tested{TimingTest}
auto timingRecordLess(const TimingRecord &left, const TimingRecord &right) -> bool {
    if (left.duration != right.duration) {
        return left.duration > right.duration;
    }
    if (left.suite != right.suite) {
        return left.suite < right.suite;
    }
    return left.test < right.test;
}

}

void TimingStatistics::addMethod(const std::string &suite,
    const std::string &test,
    const TimingKind kind,
    const TestDuration duration,
    const bool interrupted) {
    _methods.emplace_back(TimingRecord{suite, test, kind, duration, interrupted});
}

void TimingStatistics::addSuite(const std::string &suite, const TestDuration duration, const bool interrupted) {
    _suites.emplace_back(TimingRecord{suite, {}, TimingKind::Suite, duration, interrupted});
}

auto TimingStatistics::tests() const -> TimingSummary {
    auto records = std::vector<TimingRecord>{};
    records.reserve(_methods.size());
    std::ranges::copy_if(_methods, std::back_inserter(records), [](const TimingRecord &record) {
        return record.kind == TimingKind::Test;
    });
    return summarize(std::move(records));
}

auto TimingStatistics::suites() const -> TimingSummary {
    return summarize(_suites);
}

auto TimingStatistics::summarize(std::vector<TimingRecord> records) -> TimingSummary {
    auto result = TimingSummary{};
    result.count = records.size();
    if (records.empty()) {
        return result;
    }
    result.total =
        std::accumulate(records.begin(), records.end(), TestDuration{}, [](const auto total, const auto &entry) {
            return total + entry.duration;
        });
    result.average = result.total / static_cast<double>(result.count);

    auto durations = std::vector<TestDuration>{};
    durations.reserve(records.size());
    std::ranges::transform(records, std::back_inserter(durations), &TimingRecord::duration);
    std::ranges::sort(durations);
    const auto middle = durations.size() / 2;
    if (durations.size() % 2 == 0) {
        result.median = (durations[middle - 1] + durations[middle]) / 2.0;
    } else {
        result.median = durations[middle];
    }

    std::ranges::sort(records, timingRecordLess);
    if (records.size() > cSlowestCount) {
        records.resize(cSlowestCount);
    }
    result.slowest = std::move(records);
    return result;
}

auto formatTestDuration(TestDuration duration) -> std::string {
    if (duration.count() <= 0.0) {
        return "0ms";
    }
    if (duration < TestDuration{0.000001}) {
        return "<0.001ms";
    }

    auto value = duration.count() / cTimeUnits.front().seconds;
    for (std::size_t index = 0; index < cTimeUnits.size(); ++index) {
        const auto &unit = cTimeUnits[index];
        auto rounded = roundSignificant(value);
        if (unit.limit > 0.0 && rounded >= unit.limit) {
            value = rounded / unit.limit;
            continue;
        }
        return formatSignificant(rounded) + std::string{unit.suffix};
    }
    return {};
}

}
