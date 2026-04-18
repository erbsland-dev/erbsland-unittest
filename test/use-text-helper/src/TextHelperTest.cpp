// Copyright © 2025 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later


#include <erbsland/unittest/TextHelper.hpp>
#include <erbsland/unittest/UnitTest.hpp>


namespace th = erbsland::unittest::th;


class TextHelperTest final : public el::UnitTest {
public:
    void testCharacterCount() {
        auto count = th::characterCount("abc→😀");
        REQUIRE_EQUAL(count, 5);
        count = th::characterCount(u"abc→😀");
        REQUIRE_EQUAL(count, 5);
        count = th::characterCount(U"abc→😀");
        REQUIRE_EQUAL(count, 5);
        count = th::characterCount(L"abc→😀");
        REQUIRE_EQUAL(count, 5);
        auto str = std::string("abc→😀");
        count = th::characterCount(str);
        REQUIRE_EQUAL(count, 5);
        auto u16str = std::u16string(u"abc→😀");
        count = th::characterCount(u16str);
        REQUIRE_EQUAL(count, 5);
        auto u32str = std::u32string(U"abc→😀");
        count = th::characterCount(u32str);
        REQUIRE_EQUAL(count, 5);
        auto w_str = std::wstring(L"abc→😀");
        count = th::characterCount(w_str);
        REQUIRE_EQUAL(count, 5);
    }

    void testToConsoleSafeString() {
        const auto expected1 = std::string(R"(abc#$*09')");
        const auto expected2 = std::string(R"("abc\n\r\t\\\"\u{1234}\x01")");
        auto safeStr1 = th::toConsoleSafeString("abc#$*09'", 100U);
        REQUIRE_EQUAL(safeStr1, expected1);
        auto safeStr2 = th::toConsoleSafeString("abc\n\r\t\\\"\u1234\x01", 100U);
        REQUIRE_EQUAL(safeStr2, expected2);
        safeStr1 = th::toConsoleSafeString(u"abc#$*09'", 100U);
        REQUIRE_EQUAL(safeStr1, expected1);
        safeStr2 = th::toConsoleSafeString(u"abc\n\r\t\\\"\u1234\x01", 100U);
        REQUIRE_EQUAL(safeStr2, expected2);
        safeStr1 = th::toConsoleSafeString(U"abc#$*09'", 100U);
        REQUIRE_EQUAL(safeStr1, expected1);
        safeStr2 = th::toConsoleSafeString(U"abc\n\r\t\\\"\u1234\x01", 100U);
        REQUIRE_EQUAL(safeStr2, expected2);
        safeStr1 = th::toConsoleSafeString(L"abc#$*09'", 100U);
        REQUIRE_EQUAL(safeStr1, expected1);
        safeStr2 = th::toConsoleSafeString(L"abc\n\r\t\\\"\u1234\x01", 100U);
        REQUIRE_EQUAL(safeStr2, expected2);

        auto croppedString = th::toConsoleSafeString(std::string(5000U, 'x'), 20U);
        REQUIRE(croppedString.find("more") != std::string::npos);
        REQUIRE_LESS_EQUAL(croppedString.size(), 40U);
    }

    void testToStdString() {
        const auto expected = std::string("abc😄×↑");
        auto str = th::toStdString("abc😄×↑");
        REQUIRE_EQUAL(str, expected);
        str = th::toStdString(u"abc😄×↑");
        REQUIRE_EQUAL(str, expected);
        str = th::toStdString(U"abc😄×↑");
        REQUIRE_EQUAL(str, expected);
        str = th::toStdString(L"abc😄×↑");
        REQUIRE_EQUAL(str, expected);
    }

    void testToStdU32String() {
        const auto expected = std::u32string(U"abc😄×↑");
        auto str = th::toStdU32String("abc😄×↑");
        REQUIRE_EQUAL(str, expected);
        str = th::toStdU32String(u8"abc😄×↑");
        REQUIRE_EQUAL(str, expected);
        str = th::toStdU32String(u"abc😄×↑");
        REQUIRE_EQUAL(str, expected);
        str = th::toStdU32String(U"abc😄×↑");
        REQUIRE_EQUAL(str, expected);
        str = th::toStdU32String(L"abc😄×↑");
        REQUIRE_EQUAL(str, expected);

        auto invalidUtf8 = std::string{"A"};
        invalidUtf8.push_back(static_cast<char>(0x80));
        invalidUtf8.push_back('B');
        REQUIRE_EQUAL(th::toStdU32String(invalidUtf8), std::u32string(U"A\uFFFDB"));
    }

    void testStringFromHex() {
        auto expectedBytes = std::string{};
        expectedBytes.push_back(static_cast<char>(0x01));
        expectedBytes.push_back(static_cast<char>(0x02));
        expectedBytes.push_back(static_cast<char>(0xA1));
        REQUIRE_EQUAL(th::stdStringFromHex("0102A1"), expectedBytes);
        REQUIRE_EQUAL(th::stdStringFromHex("01 02    a1"), expectedBytes);

        auto expectedU8 = std::u8string{};
        expectedU8.push_back(static_cast<char8_t>(0x00));
        expectedU8.push_back(static_cast<char8_t>(0x7F));
        expectedU8.push_back(static_cast<char8_t>(0xFF));
        REQUIRE_EQUAL(th::stdU8StringFromHex("00 7f FF"), expectedU8);

        const auto expectedU16 = std::u16string{char16_t{0x56A9}, char16_t{0x12FF}};
        REQUIRE_EQUAL(th::stdU16StringFromHex("56a9 12FF"), expectedU16);

        const auto expectedU32 = std::u32string{char32_t{0x12345678}, char32_t{0xABC49283}};
        REQUIRE_EQUAL(th::stdU32StringFromHex("12345678 abc49283"), expectedU32);

        if constexpr (sizeof(wchar_t) == 2) {
            const auto expectedWStr = std::wstring{wchar_t{0x56A9}, wchar_t{0x12FF}};
            REQUIRE_EQUAL(th::stdWStringFromHex("56a9 12ff"), expectedWStr);
        } else {
            const auto expectedWStr = std::wstring{
                static_cast<wchar_t>(0x12345678),
                static_cast<wchar_t>(0x1BC49283)};
            REQUIRE_EQUAL(th::stdWStringFromHex("12345678 1BC49283"), expectedWStr);
        }

        REQUIRE_THROWS_AS(std::logic_error, th::stdStringFromHex("0"));
        REQUIRE_THROWS_AS(std::logic_error, th::stdStringFromHex("01 "));
        REQUIRE_THROWS_AS(std::logic_error, th::stdStringFromHex(" 01"));
        REQUIRE_THROWS_AS(std::logic_error, th::stdStringFromHex("01 0G"));
        REQUIRE_THROWS_AS(std::logic_error, th::stdU16StringFromHex("56 a9"));
    }

    void testRequireValidUtf8() {
        REQUIRE_VALID_UTF8("abc😄×↑");
        REQUIRE_VALID_UTF8(u8"abc😄×↑");

        auto invalidContinuation = std::string{"A"};
        invalidContinuation.push_back(static_cast<char>(0x80));
        invalidContinuation.push_back('B');
        const auto invalidContinuationResult = th::impl::validateUtf8(invalidContinuation);
        REQUIRE_FALSE(invalidContinuationResult.valid);
        REQUIRE_EQUAL(invalidContinuationResult.byteIndex, std::size_t{1});

        const auto surrogateUtf8 = std::string{"\xED\xA0\x80", 3};
        const auto surrogateUtf8Result = th::impl::validateUtf8(surrogateUtf8);
        REQUIRE_FALSE(surrogateUtf8Result.valid);
        REQUIRE_EQUAL(surrogateUtf8Result.byteIndex, std::size_t{0});
    }

    void testInvalidUtf8Generator() {
        for (const auto error : th::allUtf8Errors) {
            const auto text = th::invalidUtf8(error, "pre", "suf");
            REQUIRE_EQUAL(text.substr(0, 3), "pre");
            REQUIRE_EQUAL(text.substr(text.size() - 3), "suf");
            const auto validation = th::impl::validateUtf8(text);
            REQUIRE_FALSE(validation.valid);
            REQUIRE_EQUAL(validation.byteIndex, std::size_t{3});
        }
    }

    void testSplitLines() {
        const auto lineViews = th::splitLineViews("\nalpha\n\nbeta\n");
        REQUIRE_EQUAL(lineViews.size(), std::size_t{4});
        REQUIRE(lineViews[0].empty());
        REQUIRE_EQUAL(lineViews[1], "alpha");
        REQUIRE(lineViews[2].empty());
        REQUIRE_EQUAL(lineViews[3], "beta");

        const auto u16Lines = th::splitLines(u"alpha\n\nbeta\n");
        REQUIRE_EQUAL(u16Lines.size(), std::size_t{3});
        REQUIRE_EQUAL(u16Lines[0], u"alpha");
        REQUIRE(u16Lines[1].empty());
        REQUIRE_EQUAL(u16Lines[2], u"beta");

        const auto u32Lines = th::splitLines(U"alpha\nbeta");
        REQUIRE_EQUAL(u32Lines.size(), std::size_t{2});
        REQUIRE_EQUAL(u32Lines[0], U"alpha");
        REQUIRE_EQUAL(u32Lines[1], U"beta");
    }

    void testRequireEqualLines() {
        const auto expected = std::vector<std::string_view>{
            "hello one two three",
            "anything*",
            "*anything",
            "anything*anything",
            "two??wildcards",
            "*any??",
        };
        const auto actual = std::vector<std::string_view>{
            "hello one two three",
            "anything→goes.here",
            "another line with anything",
            "anything can be in the middle, anything",
            "two+-wildcards",
            "is there any::",
        };
        // manual use
        WITH_CONTEXT(th::requireEqualLines(*this, actual, expected));
        // macro use
        REQUIRE_EQUAL_LINES(actual, expected);
    }

    void testSideBySideComparison() {
        const auto expected = std::vector<std::string>{
            "one two three four five six",
            "another line",
            "last line",
        };
        const auto actual = std::vector<std::string>{
            "one two three four five six",
            "different",
        };
        auto comparisonStr = th::impl::createSideBySideComparison(actual, expected);
        const auto expectedComparisonStr = th::splitLines(
            "| Actual (2)                  |     | Expected (3)                |\n"
            "|-----------------------------|-----|-----------------------------|\n"
            "| one two three four five six | === | one two three four five six |\n"
            "| different                   |  X  | another line                |\n"
            "|                             |  X  | last line                   |\n"
            "|-----------------------------|-----|-----------------------------|");
        const auto comparisonLines = th::splitLines(comparisonStr);
        REQUIRE_EQUAL(comparisonLines.size(), expectedComparisonStr.size());
        for (auto index = std::size_t{0}; index < comparisonLines.size(); ++index) {
            WITH_CONTEXT(index);
            REQUIRE_EQUAL(comparisonLines[index], expectedComparisonStr[index]);
        }
    }
};
