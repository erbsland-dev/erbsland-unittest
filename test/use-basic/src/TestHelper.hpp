// Copyright © 2025 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include <erbsland/unittest/UnitTest.hpp>


class TestHelper : public el::UnitTest {
public:
    /// Test all operators
    ///
    /// Pass six arguments that are:
    /// a1 == b1, a2 == b2, a3 == b3
    /// a1 < a2, a2 < a3, b1 < b2, b2 < b3
    ///
    /// The test always puts `a<n>` on the left, and `b<n>` on the right side of the operator.
    ///
    template<typename A, typename B>
    void requireAllOperators(
        const A &a1, const A &a2, const A &a3,
        const B &b1, const B &b2, const B &b3) {

        // Test ==
        REQUIRE(a1 == b1);
        REQUIRE_FALSE(a1 == b2);
        REQUIRE_FALSE(a1 == b3);
        REQUIRE_FALSE(a2 == b1);
        REQUIRE(a2 == b2);
        REQUIRE_FALSE(a2 == b3);
        REQUIRE_FALSE(a3 == b1);
        REQUIRE_FALSE(a3 == b2);
        REQUIRE(a3 == b3);

        // Test !=
        REQUIRE_FALSE(a1 != b1);
        REQUIRE(a1 != b2);
        REQUIRE(a1 != b3);
        REQUIRE(a2 != b1);
        REQUIRE_FALSE(a2 != b2);
        REQUIRE(a2 != b3);
        REQUIRE(a3 != b1);
        REQUIRE(a3 != b2);
        REQUIRE_FALSE(a3 != b3);

        // Test <
        REQUIRE_FALSE(a1 < b1);
        REQUIRE(a1 < b2);
        REQUIRE(a1 < b3);
        REQUIRE_FALSE(a2 < b1);
        REQUIRE_FALSE(a2 < b2);
        REQUIRE(a2 < b3);
        REQUIRE_FALSE(a3 < b1);
        REQUIRE_FALSE(a3 < b2);
        REQUIRE_FALSE(a3 < b3);

        // Test <=
        REQUIRE(a1 <= b1);
        REQUIRE(a1 <= b2);
        REQUIRE(a1 <= b3);
        REQUIRE_FALSE(a2 <= b1);
        REQUIRE(a2 <= b2);
        REQUIRE(a2 <= b3);
        REQUIRE_FALSE(a3 <= b1);
        REQUIRE_FALSE(a3 <= b2);
        REQUIRE(a3 <= b3);

        // Test >
        REQUIRE_FALSE(a1 > b1);
        REQUIRE_FALSE(a1 > b2);
        REQUIRE_FALSE(a1 > b3);
        REQUIRE(a2 > b1);
        REQUIRE_FALSE(a2 > b2);
        REQUIRE_FALSE(a2 > b3);
        REQUIRE(a3 > b1);
        REQUIRE(a3 > b2);
        REQUIRE_FALSE(a3 > b3);

        // Test >=
        REQUIRE(a1 >= b1);
        REQUIRE_FALSE(a1 >= b2);
        REQUIRE_FALSE(a1 >= b3);
        REQUIRE(a2 >= b1);
        REQUIRE(a2 >= b2);
        REQUIRE_FALSE(a2 >= b3);
        REQUIRE(a3 >= b1);
        REQUIRE(a3 >= b2);
        REQUIRE(a3 >= b3);

        // Test <=>
        REQUIRE((a1 <=> b1) == std::strong_ordering::equal);
        REQUIRE((a1 <=> b2) == std::strong_ordering::less);
        REQUIRE((a1 <=> b3) == std::strong_ordering::less);
        REQUIRE((a2 <=> b1) == std::strong_ordering::greater);
        REQUIRE((a2 <=> b2) == std::strong_ordering::equal);
        REQUIRE((a2 <=> b3) == std::strong_ordering::less);
        REQUIRE((a3 <=> b1) == std::strong_ordering::greater);
        REQUIRE((a3 <=> b2) == std::strong_ordering::greater);
        REQUIRE((a3 <=> b3) == std::strong_ordering::equal);
    }

    template<typename T, std::size_t tSize>
    void requireStrictOrder(const std::array<T, tSize> &valuesInOrder) {
        for (std::size_t i = 0; i < tSize; ++i) {
            for (std::size_t j = 0; j < tSize; ++j) {
                const auto &iValue = valuesInOrder[i];
                const auto &jValue = valuesInOrder[j];
                auto isEqual = i == j;
                auto isLess = i < j;
                auto isLessOrEqual = i <= j;
                auto isGreater = i > j;
                auto isGreaterOrEqual = i >= j;
                REQUIRE(isEqual == (iValue == jValue));
                REQUIRE(isLess == (iValue < jValue));
                REQUIRE(isLessOrEqual == (iValue <= jValue));
                REQUIRE(isGreater == (iValue > jValue));
                REQUIRE(isGreaterOrEqual == (iValue >= jValue));
                REQUIRE((iValue <=> jValue) == (i <=> j));
            }
        }
    }
};

