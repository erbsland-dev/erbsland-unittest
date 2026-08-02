// Copyright (c) 2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0

#include <erbsland/unittest/impl/Timing.hpp>
#include <erbsland/unittest/UnitTest.hpp>

#include <chrono>
#include <format>

namespace {

using erbsland::unittest::formatTestDuration;
using erbsland::unittest::TestDuration;
using erbsland::unittest::TimingKind;
using erbsland::unittest::TimingStatistics;
using namespace std::chrono_literals;

}

TESTED_TARGETS(TimingStatistics formatTestDuration)
class TimingTest final : public el::UnitTest {
public:
    void testDurationFormatting() {
        REQUIRE_EQUAL(formatTestDuration(TestDuration{}), "0ms");
        REQUIRE_EQUAL(formatTestDuration(1ns), "<0.001ms");
        REQUIRE_EQUAL(formatTestDuration(1us), "0.00100ms");
        REQUIRE_EQUAL(formatTestDuration(230ms), "230ms");
        REQUIRE_EQUAL(formatTestDuration(TestDuration{0.9994}), "999ms");
        REQUIRE_EQUAL(formatTestDuration(TestDuration{0.9996}), "1.00s");
        REQUIRE_EQUAL(formatTestDuration(TestDuration{1.25}), "1.25s");
        REQUIRE_EQUAL(formatTestDuration(TestDuration{59.94}), "59.9s");
        REQUIRE_EQUAL(formatTestDuration(TestDuration{59.96}), "1.00m");
        REQUIRE_EQUAL(formatTestDuration(150s), "2.50m");
        REQUIRE_EQUAL(formatTestDuration(3599s), "1.00h");
        REQUIRE_EQUAL(formatTestDuration(3960s), "1.10h");
        REQUIRE_EQUAL(formatTestDuration(1234h), "1230h");
    }

    void testTestSummaryAndPrintExclusion() {
        auto statistics = TimingStatistics{};
        statistics.addMethod("Example", "One", TimingKind::Test, TestDuration{1.0}, false);
        statistics.addMethod("Example", "Two", TimingKind::Test, TestDuration{2.0}, false);
        statistics.addMethod("Example", "Three", TimingKind::Test, TestDuration{3.0}, true);
        statistics.addMethod("Example", "Four", TimingKind::Test, TestDuration{4.0}, false);
        statistics.addMethod("Example", "Output", TimingKind::Print, TestDuration{100.0}, false);

        const auto summary = statistics.tests();
        REQUIRE_EQUAL(summary.count, 4U);
        REQUIRE_EQUAL(summary.total, TestDuration{10.0});
        REQUIRE_EQUAL(summary.average, TestDuration{2.5});
        REQUIRE_EQUAL(summary.median, TestDuration{2.5});
        REQUIRE_EQUAL(summary.slowest.size(), 4U);
        REQUIRE_EQUAL(summary.slowest[0].test, "Four");
        REQUIRE_EQUAL(summary.slowest[1].test, "Three");
        REQUIRE(summary.slowest[1].interrupted);
        REQUIRE_EQUAL(summary.slowest[3].test, "One");
    }

    void testOddMedian() {
        auto statistics = TimingStatistics{};
        statistics.addMethod("Example", "One", TimingKind::Test, TestDuration{1.0}, false);
        statistics.addMethod("Example", "Two", TimingKind::Test, TestDuration{9.0}, false);
        statistics.addMethod("Example", "Three", TimingKind::Test, TestDuration{3.0}, false);
        REQUIRE_EQUAL(statistics.tests().median, TestDuration{3.0});
    }

    void testSlowestLimitAndTieOrder() {
        auto statistics = TimingStatistics{};
        for (int index = 11; index >= 0; --index) {
            statistics.addMethod("Suite", std::format("Test{:02}", index), TimingKind::Test, TestDuration{1.0}, false);
        }

        const auto summary = statistics.tests();
        REQUIRE_EQUAL(summary.count, 12U);
        REQUIRE_EQUAL(summary.slowest.size(), 10U);
        REQUIRE_EQUAL(summary.slowest.front().test, "Test00");
        REQUIRE_EQUAL(summary.slowest.back().test, "Test09");
    }

    void testSuiteSummary() {
        auto statistics = TimingStatistics{};
        statistics.addSuite("Slow", TestDuration{8.0}, true);
        statistics.addSuite("Fast", TestDuration{2.0}, false);

        const auto summary = statistics.suites();
        REQUIRE_EQUAL(summary.count, 2U);
        REQUIRE_EQUAL(summary.total, TestDuration{10.0});
        REQUIRE_EQUAL(summary.average, TestDuration{5.0});
        REQUIRE_EQUAL(summary.median, TestDuration{5.0});
        REQUIRE_EQUAL(summary.slowest.front().suite, "Slow");
        REQUIRE(summary.slowest.front().interrupted);
    }

    void printTimingOutput() { consoleWriteLine("Timing print output"); }
};
