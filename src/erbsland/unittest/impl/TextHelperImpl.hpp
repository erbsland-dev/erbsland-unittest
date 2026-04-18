// Copyright © 2026 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once


#include "ConsoleLine.hpp"

#include <algorithm>
#include <cstdint>
#include <format>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

namespace erbsland::unittest::th::impl {

/// Concept that matches any supported string_view type.
template <typename T>
concept AnyStringView = std::same_as<T, std::string_view> || std::same_as<T, std::u8string_view> ||
    std::same_as<T, std::u16string_view> || std::same_as<T, std::u32string_view> || std::same_as<T, std::wstring_view>;

/// Concept that matches any supported string type.
template <typename T>
concept AnyString = std::same_as<T, std::string> || std::same_as<T, std::u8string> || std::same_as<T, std::u16string> ||
    std::same_as<T, std::u32string> || std::same_as<T, std::wstring>;

/// Concept that matches any supported string and string view type.
template <typename T>
concept AnyStringOrStringView = AnyString<T> || AnyStringView<T>;

/// Convert any string into a UTF-8 encoded `std::string`.
/// @param str The input string.
/// @return The UTF-8 encoded string.
auto toStdString(std::string_view str) -> std::string;
/// @overload
auto toStdString(std::u8string_view str) -> std::string;
/// @overload
auto toStdString(std::u16string_view str) -> std::string;
/// @overload
auto toStdString(std::u32string_view str) -> std::string;
/// @overload
auto toStdString(std::wstring_view str) -> std::string;

/// Convert any string into a UTF-32 encoded `std::u32string`.
/// @param str The input string.
/// @return The UTF-32 encoded string.
auto toStdU32String(std::string_view str) -> std::u32string;
/// @overload
auto toStdU32String(std::u8string_view str) -> std::u32string;
/// @overload
auto toStdU32String(std::u16string_view str) -> std::u32string;
/// @overload
auto toStdU32String(std::u32string_view str) -> std::u32string;
/// @overload
auto toStdU32String(std::wstring_view str) -> std::u32string;

/// Validation result for UTF-8 encoding check.
struct Utf8ValidationResult final {
    explicit Utf8ValidationResult(const bool valid = false, const std::size_t byteIndex = 0, std::string message = {}) :
        valid{valid}, byteIndex{byteIndex}, message{std::move(message)} {}

    bool valid{false};
    std::size_t byteIndex{0};
    std::string message;
};

/// Validate that the given string contains only well-formed UTF-8 code units.
/// @param str The input string.
/// @return The validation result. `valid` is true when the string is valid UTF-8.
auto validateUtf8(std::string_view str) -> Utf8ValidationResult;
/// @overload
auto validateUtf8(std::u8string_view str) -> Utf8ValidationResult;

/// Convert a hexadecimal character into its numeric value.
/// @param digit The hexadecimal digit.
/// @return The numeric value, or `-1` if it is not a valid hex digit.
auto hexDigitValue(char digit) -> int;

/// Parse a hexadecimal string into a string with fixed-width code units.
/// @param hexString The input text.
/// @param hexDigitsPerGroup The number of hex digits per group/code unit.
/// @return The parsed string.
/// @throws std::logic_error if the format is invalid.
template <typename tString>
auto stringFromHex(const std::string_view hexString, const std::size_t hexDigitsPerGroup) -> tString {
    auto result = tString{};
    result.reserve(hexString.size() / hexDigitsPerGroup);
    auto pos = std::size_t{0};
    while (pos < hexString.size()) {
        using tValue = tString::value_type;
        auto value = std::uint64_t{0};
        for (auto digitIndex = std::size_t{0}; digitIndex < hexDigitsPerGroup; ++digitIndex) {
            if (pos >= hexString.size()) {
                throw std::logic_error{"Incomplete hex group in hex string."};
            }
            const auto digit = hexString[pos];
            if (digit == ' ') {
                throw std::logic_error{"Spaces are only allowed between complete hex groups."};
            }
            const auto digitValue = hexDigitValue(digit);
            if (digitValue < 0) {
                throw std::logic_error{"Invalid hex digit in hex string."};
            }
            value = (value << 4U) | static_cast<std::uint64_t>(digitValue);
            ++pos;
        }
        result.push_back(static_cast<tValue>(value));
        if (pos >= hexString.size()) {
            break;
        }
        if (hexString[pos] == ' ') {
            while (pos < hexString.size() && hexString[pos] == ' ') {
                ++pos;
            }
            if (pos >= hexString.size()) {
                throw std::logic_error{"Trailing spaces are not allowed in hex string."};
            }
        }
    }
    return result;
}

/// Count code-points in a string.
/// @param str The input string.
/// @return The number of code-points represented by the string.
[[nodiscard]] auto characterCount(std::string_view str) noexcept -> std::size_t;
/// @overload
[[nodiscard]] auto characterCount(std::u8string_view str) noexcept -> std::size_t;
/// @overload
[[nodiscard]] auto characterCount(std::u16string_view str) noexcept -> std::size_t;
/// @overload
[[nodiscard]] auto characterCount(std::u32string_view str) noexcept -> std::size_t;
/// @overload
[[nodiscard]] auto characterCount(std::wstring_view str) noexcept -> std::size_t;

/// Create a padded string with spaces to reach a given width
/// @param text The unpadded text.
/// @param width The desired width of the padded string.
/// @return The padded string.
template <typename tStringView>
    requires std::is_convertible_v<tStringView, std::string_view>
static auto paddedString(const tStringView &text, std::size_t width) -> std::string {
    const auto characterCount = ConsoleLine::utf8Length(text);
    if (characterCount >= width) {
        return std::string{text};
    }
    std::string result;
    result.reserve(text.size() + (width - characterCount) + 1);
    result += text;
    result += std::string(width - characterCount, ' ');
    return result;
}

/// Compares a string and a pattern in reverse order, where the pattern may
/// contain wildcard characters represented by '?'.
///
/// The comparison starts from the end of both the pattern and the string and
/// progresses backward. A '?' in the pattern matches any single character
/// in the string.
///
/// @param pattern The pattern string to match, which may contain '?' as wildcards.
/// @param str The string to compare against the pattern.
/// @return True if the pattern matches the string in reverse order (considering
///         wildcards); otherwise, false.
template <AnyStringOrStringView tStringView>
[[nodiscard]] auto reverseCompareWithQuestionMark(const tStringView pattern, const tStringView str) -> bool {
    auto strIt = str.rbegin();
    auto patternIt = pattern.rbegin();
    for (; strIt != str.rend() && patternIt != pattern.rend(); ++strIt, ++patternIt) {
        if (*patternIt == static_cast<tStringView::value_type>('?')) {
            continue;
        }
        if (*strIt != *patternIt) {
            return false;
        }
    }
    return patternIt == pattern.rend();
}

/// Compares a string and a pattern where the pattern may contain wildcard characters represented by '?'.
///
/// The comparison starts from the start and progresses forward. A '?' in the pattern matches any single
/// character in the string.
///
/// @param pattern The pattern string to match, which may contain '?' as wildcards.
/// @param str The string to compare against the pattern.
/// @param fullMatch If true, the entire string must match the pattern; otherwise, a partial match is sufficient.
/// @return True if the pattern matches the string; otherwise, false.
template <AnyStringOrStringView tStringView>
[[nodiscard]] auto compareWithQuestionMark(const tStringView pattern, const tStringView str, bool fullMatch) -> bool {
    auto strIt = str.begin();
    auto patternIt = pattern.begin();
    for (; strIt != str.end() && patternIt != pattern.end(); ++strIt, ++patternIt) {
        if (*patternIt == static_cast<tStringView::value_type>('?')) {
            continue;
        }
        if (*strIt != *patternIt) {
            return false;
        }
    }
    if (fullMatch && strIt != str.end()) {
        return false;
    }
    if (patternIt != pattern.end()) {
        return false;
    }
    return true;
}

/// Compare a pattern that contains a wildcard '*' or '?'.
///
/// The pattern may contain *one* '*' or one or more '?' wildcards.
///
/// @param pattern The pattern string to match. May contain *one* '*' or one or more '?' wildcards.
/// @param str The string to compare against the pattern.
/// @return True if the pattern matches the string; otherwise, false.
template <AnyStringOrStringView tStringView>
[[nodiscard]] auto compareWithStarAndQuestionMark(const tStringView pattern, const tStringView str) -> bool {
    const auto pos = pattern.find(static_cast<tStringView::value_type>('*'));
    if (pos == std::string_view::npos) {
        return compareWithQuestionMark(pattern, str, true);
    }
    const auto prefix = pattern.substr(0, pos);
    if (!compareWithQuestionMark(prefix, str, false)) {
        return false;
    }
    const auto suffix = pattern.substr(pos + 1);
    if (suffix.empty()) {
        return true;
    }
    return reverseCompareWithQuestionMark(suffix, str);
}

/// Create a side-by-side comparison string from two line ranges.
template <typename tActual, typename tExpected>
auto createSideBySideComparison(const tActual &actual, const tExpected &expected) -> std::string {
    // display a side-by-side comparison.
    std::string message;
    message.reserve(2048);
    std::size_t actualWidth = 1;
    if (actual.size() > 0) {
        actualWidth =
            characterCount(*std::max_element(actual.begin(), actual.end(), [](const auto &a, const auto &b) -> bool {
                return characterCount(a) < characterCount(b);
            }));
    }
    std::size_t expectedWidth = 1;
    if (expected.size() > 0) {
        expectedWidth = characterCount(
            *std::max_element(expected.begin(), expected.end(), [](const auto &a, const auto &b) -> bool {
                return characterCount(a) < characterCount(b);
            }));
    }
    const auto actualHeader = std::format("Actual ({})", actual.size());
    const auto expectedHeader = std::format("Expected ({})", expected.size());
    actualWidth = std::clamp(actualWidth, actualHeader.size(), std::size_t{200U});
    expectedWidth = std::clamp(expectedWidth, expectedHeader.size(), std::size_t{200U});
    message += std::string{"| "};
    message += paddedString(actualHeader, actualWidth);
    message += " |     | ";
    message += paddedString(expectedHeader, expectedWidth);
    message += " |\n";
    message += std::string{"|-"};
    message += std::string(actualWidth, '-');
    message += "-|-----|-";
    message += std::string(expectedWidth, '-');
    message += "-|\n";
    auto itActual = actual.begin();
    auto itExpected = expected.begin();
    while (itActual != actual.end() || itExpected != expected.end()) {
        message += std::string{"| "};
        if (itActual != actual.end()) {
            message += paddedString(toStdString(*itActual), actualWidth);
        } else {
            message += paddedString(std::string{}, actualWidth);
        }

        const auto areEqual = itActual != actual.end() && itExpected != expected.end() && *itActual == *itExpected;
        if (areEqual) {
            message += " | === | ";
        } else {
            message += " |  X  | ";
        }
        if (itExpected != expected.end()) {
            message += paddedString(toStdString(*itExpected), expectedWidth);
        } else {
            message += paddedString(std::string{}, expectedWidth);
        }
        message += " |\n";

        if (itActual != actual.end()) {
            ++itActual;
        }
        if (itExpected != expected.end()) {
            ++itExpected;
        }
    }
    message += std::string{"|-"};
    message += std::string(actualWidth, '-');
    message += "-|-----|-";
    message += std::string(expectedWidth, '-');
    message += "-|\n";
    return message;
}

template <AnyStringView tStringView>
auto splitLineViews(const tStringView text) -> std::vector<tStringView> {
    auto result = std::vector<tStringView>{};
    if (text.empty()) {
        return result;
    }
    auto lineStart = std::size_t{0};
    while (true) {
        const auto nextLineBreak = text.find(static_cast<typename tStringView::value_type>('\n'), lineStart);
        if (nextLineBreak == tStringView::npos) {
            break;
        }
        result.push_back(text.substr(lineStart, nextLineBreak - lineStart));
        lineStart = nextLineBreak + 1;
    }
    if (lineStart < text.size()) {
        result.push_back(text.substr(lineStart));
    }
    return result;
}

template <AnyStringView tStringView, typename tString>
auto splitLines(const tStringView text) -> std::vector<tString> {
    auto result = std::vector<tString>{};
    for (const auto line : splitLineViews(text)) {
        result.push_back(tString{line});
    }
    return result;
}

}
