// Copyright © 2025 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later


#include "TestHelper.hpp"

#include <ExamplePriority.hpp>

#include <array>
#include <string>
#include <vector>


using erbsland::ExamplePriority;


TESTED_TARGETS(ExamplePriority)
class PriorityTest final : public UNITTEST_SUBCLASS(TestHelper) {
public:
    ExamplePriority examplePriority{ExamplePriority::Lowest};

    void testConstruction() {
        examplePriority = ExamplePriority{ExamplePriority::Highest};
        REQUIRE(examplePriority == ExamplePriority::Highest);
        examplePriority = ExamplePriority::Medium;
        REQUIRE(examplePriority == ExamplePriority::Medium);
    }

    void testComparison() {
        const auto p1 = ExamplePriority{ExamplePriority::Lowest};
        const auto p2 = ExamplePriority{ExamplePriority::Low};
        const auto p3 = ExamplePriority{ExamplePriority::Highest};
        WITH_CONTEXT(requireAllOperators(p1, p2, p3, p1, p2, p3));
    }

    void testOrdering() {
        const auto sequence = std::array<ExamplePriority, 5>{
            ExamplePriority::Lowest,
            ExamplePriority::Low,
            ExamplePriority::Medium,
            ExamplePriority::High,
            ExamplePriority::Highest
        };
        WITH_CONTEXT(requireStrictOrder(sequence));
    }

    void testToString() {
        struct TestData {
            ExamplePriority priority;
            std::string expectedString;
        };
        const auto testData = std::vector<TestData>{
            {ExamplePriority::Lowest, "Lowest"},
            {ExamplePriority::Low, "Low"},
            {ExamplePriority::Medium, "Medium"},
            {ExamplePriority::High, "High"},
            {ExamplePriority::Highest, "Highest"}
        };
        for (const auto &[priority, expectedString] : testData) {
            REQUIRE_EQUAL(priority.toString(), expectedString);
        }
    }

    void testFromString() {
        struct TestData {
            std::string string;
            ExamplePriority expectedPriority;
        };
        const auto testData = std::vector<TestData>{
            {"Lowest", ExamplePriority::Lowest},
            {"Low", ExamplePriority::Low},
            {"Medium", ExamplePriority::Medium},
            {"High", ExamplePriority::High},
            {"Highest", ExamplePriority::Highest}
        };
        for (const auto &[string, expectedPriority] : testData) {
            REQUIRE_EQUAL(ExamplePriority::fromString(string), expectedPriority);
        }
    }
};

