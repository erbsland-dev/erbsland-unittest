// Copyright © 2025 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "UnitTest.hpp"

#include "impl/Macros.hpp"
#include "impl/TextHelperImpl.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>


/// Namespace for text helpers.
///
/// These text helper functions are mainly provided for handling diagnostic output and validating text in
/// unit tests. They are designed to be robust but can be inefficient, as they internally convert the text
/// to UTF-8 for processing, which can be costly for large strings.
///
/// For performance critical hot-paths in your tests, please use a custom implementation that avoids the
/// UTF-8 conversion.
namespace erbsland::unittest::th {

/// All supported malformed UTF-8 sequence categories for `invalidUtf8()`.
enum class Utf8Error : uint8_t {
    UnexpectedContinuationByte, ///< A standalone continuation byte (`0x80`-`0xBF`) without a valid start byte.
    Overlong2ByteSequence, ///< A two-byte sequence that encodes a value representable in one byte.
    Truncated2ByteSequence, ///< A two-byte sequence whose continuation byte is missing.
    InvalidContinuationByteIn2ByteSequence, ///< A two-byte sequence followed by a non-continuation byte.
    Overlong3ByteSequence, ///< A three-byte sequence that uses more bytes than required for the code-point.
    Truncated3ByteSequence, ///< A three-byte sequence with one or more missing continuation bytes.
    InvalidContinuationByteIn3ByteSequence, ///< A three-byte sequence containing an invalid continuation byte.
    SurrogateCodePoint, ///< A UTF-8 sequence that decodes to a UTF-16 surrogate code-point.
    Overlong4ByteSequence, ///< A four-byte sequence that over-encodes a smaller code-point.
    Truncated4ByteSequence, ///< A four-byte sequence with one or more missing continuation bytes.
    InvalidContinuationByteIn4ByteSequence, ///< A four-byte sequence containing an invalid continuation byte.
    CodePointBeyondUnicodeRange, ///< A UTF-8 sequence that decodes to a code-point above `U+10FFFF`.
    InvalidStartByte, ///< A byte that cannot start a valid UTF-8 sequence.

    _count,
};

/// A list of all malformed UTF-8 categories supported by `invalidUtf8()`.
///
/// This constant is convenient for parameterized tests that verify UTF-8 validation or error reporting
/// against every malformed sequence generator provided by this API.
constexpr auto allUtf8Errors = std::array<Utf8Error, static_cast<std::size_t>(Utf8Error::_count)>{
    Utf8Error::UnexpectedContinuationByte,
    Utf8Error::Overlong2ByteSequence,
    Utf8Error::Truncated2ByteSequence,
    Utf8Error::InvalidContinuationByteIn2ByteSequence,
    Utf8Error::Overlong3ByteSequence,
    Utf8Error::Truncated3ByteSequence,
    Utf8Error::InvalidContinuationByteIn3ByteSequence,
    Utf8Error::SurrogateCodePoint,
    Utf8Error::Overlong4ByteSequence,
    Utf8Error::Truncated4ByteSequence,
    Utf8Error::InvalidContinuationByteIn4ByteSequence,
    Utf8Error::CodePointBeyondUnicodeRange,
    Utf8Error::InvalidStartByte,
};

/// Get the code-point count for UTF-8 encoded text.
/// @param text The UTF-8 encoded text to count the code-points for.
/// @return The number of code-points in the UTF-8 encoded text.
[[nodiscard]] auto characterCount(std::string_view text) noexcept -> std::size_t;
/// @overload
[[nodiscard]] auto characterCount(std::u8string_view text) noexcept -> std::size_t;
/// @overload
[[nodiscard]] auto characterCount(std::u16string_view text) noexcept -> std::size_t;
/// @overload
[[nodiscard]] auto characterCount(std::u32string_view text) noexcept -> std::size_t;
/// @overload
[[nodiscard]] auto characterCount(std::wstring_view text) noexcept -> std::size_t;

/// Create text that is safe for console output from UTF-8 encoded input.
///
/// This call scans the UTF-8 encoded text for any invalid UTF-8 sequences and replaces the invalid bytes
/// with escape sequences "\x<XX>". Valid UTF-8 sequences are preserved, but all code-points >=0x80 are
/// escaped as "\u{<XXXX>}" and control codes are escaped as "\x<XX>" or as "\n", "\r", "\t".
/// Any backslash and double quote are escaped as "\\" or "\"".
///
/// If the string contains spaces, double quotes, or escape sequences - it is automatically enclosed in quotes.
///
/// @param text The UTF-8 encoded text to convert to console safe text.
/// @param maxLength The maximum length of the returned string. If the text is longer, it will be truncated
///    and the text "(... +<count> more)" will be added to at the end. This maximum is for the processed
///    text, containing the escape sequences.
/// @return The console safe text.
[[nodiscard]] auto toConsoleSafeString(std::string_view text, std::size_t maxLength) noexcept -> std::string;
/// @overload
[[nodiscard]] auto toConsoleSafeString(std::u8string_view text, std::size_t maxLength) noexcept -> std::string;
/// @overload
[[nodiscard]] auto toConsoleSafeString(std::u16string_view text, std::size_t maxLength) noexcept -> std::string;
/// @overload
[[nodiscard]] auto toConsoleSafeString(std::u32string_view text, std::size_t maxLength) noexcept -> std::string;
/// @overload
[[nodiscard]] auto toConsoleSafeString(std::wstring_view text, std::size_t maxLength) noexcept -> std::string;

/// Convert a string into a UTF-8 encoded `std::string`.
///
/// `std::string_view` and `std::u8string_view` are copied byte-for-byte.
/// `std::u16string_view`, `std::u32string_view` and `std::wstring_view` are transcoded to UTF-8.
[[nodiscard]] auto toStdString(std::string_view text) -> std::string;
/// @overload
[[nodiscard]] auto toStdString(std::u8string_view text) -> std::string;
/// @overload
[[nodiscard]] auto toStdString(std::u16string_view text) -> std::string;
/// @overload
[[nodiscard]] auto toStdString(std::u32string_view text) -> std::string;
/// @overload
[[nodiscard]] auto toStdString(std::wstring_view text) -> std::string;

/// Convert a string into a UTF-32 encoded `std::u32string`.
///
/// `std::u32string_view` is copied unmodified.
/// All other string views are decoded according to their encoding and converted to UTF-32 code-points.
/// Invalid UTF-8 input is replaced with the Unicode replacement character (`U+FFFD`).
[[nodiscard]] auto toStdU32String(std::string_view text) -> std::u32string;
/// @overload
[[nodiscard]] auto toStdU32String(std::u8string_view text) -> std::u32string;
/// @overload
[[nodiscard]] auto toStdU32String(std::u16string_view text) -> std::u32string;
/// @overload
[[nodiscard]] auto toStdU32String(std::u32string_view text) -> std::u32string;
/// @overload
[[nodiscard]] auto toStdU32String(std::wstring_view text) -> std::u32string;

/// Parse a hexadecimal byte string into a `std::string`.
///
/// Spaces are allowed only between complete byte groups, not inside them. Examples:
/// `"0102c8"` or `"01 02 A1"`.
///
/// @param hexString The hexadecimal string to parse.
/// @return The decoded string.
/// @throws std::logic_error if the format is invalid.
[[nodiscard]] auto stdStringFromHex(std::string_view hexString) -> std::string;
/// @overload
/// Parses two hexadecimal digits per element into a `std::u8string`.
[[nodiscard]] auto stdU8StringFromHex(std::string_view hexString) -> std::u8string;
/// @overload
/// Parses four hexadecimal digits per element into a `std::u16string`.
[[nodiscard]] auto stdU16StringFromHex(std::string_view hexString) -> std::u16string;
/// @overload
/// Parses eight hexadecimal digits per element into a `std::u32string`.
[[nodiscard]] auto stdU32StringFromHex(std::string_view hexString) -> std::u32string;
/// @overload
/// Parses one `wchar_t` value per element using `sizeof(wchar_t) * 2` hexadecimal digits.
[[nodiscard]] auto stdWStringFromHex(std::string_view hexString) -> std::wstring;

/// Generate a malformed UTF-8 byte sequence, optionally wrapped with valid prefix and suffix text.
///
/// This helper is intended for UTF-8 decoder and validator tests. The generated malformed sequence is inserted
/// between the optional prefix and suffix without modifying those surrounding bytes.
///
/// @param error The malformed UTF-8 error kind to generate.
/// @param prefix Optional valid UTF-8 bytes to prepend.
/// @param suffix Optional valid UTF-8 bytes to append.
/// @return A string containing the malformed UTF-8 sequence.
[[nodiscard]] auto invalidUtf8(
    Utf8Error error,
    const std::optional<std::string_view> &prefix = std::nullopt,
    const std::optional<std::string_view> &suffix = std::nullopt) -> std::string;

/// Split a string into individual lines without copying the line contents.
///
/// This function returns string views into the original input and therefore does not allocate storage for the
/// individual line contents. The original text must stay alive and unchanged while the returned views are used.
/// Lines are split at newline characters (`\n`), and the resulting views do not include the newline characters.
/// A trailing newline does not produce an additional empty line at the end.
///
/// @param text The text to split into lines.
/// @return A vector of string views, each representing a line from the input text.
[[nodiscard]] auto splitLineViews(std::string_view text) -> std::vector<std::string_view>;
/// @overload
[[nodiscard]] auto splitLineViews(std::u8string_view text) -> std::vector<std::u8string_view>;
/// @overload
[[nodiscard]] auto splitLineViews(std::u16string_view text) -> std::vector<std::u16string_view>;
/// @overload
[[nodiscard]] auto splitLineViews(std::u32string_view text) -> std::vector<std::u32string_view>;
/// @overload
[[nodiscard]] auto splitLineViews(std::wstring_view text) -> std::vector<std::wstring_view>;

/// Split a string into individual lines and return owned strings for each line.
///
/// Lines are split at newline characters (`\n`), and the resulting strings do not include the newline
/// characters. A trailing newline does not produce an additional empty line at the end.
///
/// @param text The text to split into lines.
/// @return A vector containing one string per line from the input text.
[[nodiscard]] auto splitLines(std::string_view text) -> std::vector<std::string>;
/// @overload
[[nodiscard]] auto splitLines(std::u8string_view text) -> std::vector<std::u8string>;
/// @overload
[[nodiscard]] auto splitLines(std::u16string_view text) -> std::vector<std::u16string>;
/// @overload
[[nodiscard]] auto splitLines(std::u32string_view text) -> std::vector<std::u32string>;
/// @overload
[[nodiscard]] auto splitLines(std::wstring_view text) -> std::vector<std::wstring>;

/// Compare a container with lines, against a container with line patterns.
///
/// Common usage:
/// <code>
/// void testLines() {
///     // ...
///     std::vector<std::string> actual = //... from test
///     auto expected = std::vector<std::string>{
///         "the first line",
///         "a second line"};
///     REQUIRE_EQUAL_LINES(actual, expected);
/// }
/// </code>
///
/// Each line on the expected side can contain *one* optional asterisk (*) character, that matches any number
/// of characters at this location. The expected line can also contain any number of question mark (?) characters
/// that match exactly one character at this location.
///
/// If the number of lines differs, or if at least one line does not match, the test fails and reports a
/// side-by-side comparison of the actual and expected lines.
///
/// @tparam tActual A range type that provides `size()`, `begin()`, `end()`, and string-like line values.
/// @tparam tExpected A range type that provides `size()`, `begin()`, `end()`, and pattern line values.
/// @param test The active test instance.
/// @param actual The produced lines from the test.
/// @param expected The expected lines or line patterns.
template <typename tActual, typename tExpected>
    requires std::ranges::range<tActual> && std::ranges::range<tExpected>
void requireEqualLines(UnitTest &test, const tActual &actual, const tExpected &expected) {

    test.runWithContext(
        SOURCE_LOCATION(),
        [&]() -> void {
            ASSERT_CONTEXT_COMPARISON_FOR_TEST(&test, "REQUIRE_EQUAL", 0, ==, actual.size(), expected.size());
            auto itActual = actual.begin();
            auto itExpected = expected.begin();
            for (; itActual != actual.end(); ++itActual, ++itExpected) {
                ASSERT_CONTEXT_REQUIRE_FOR_TEST(
                    &test, "REQUIRE", 0, impl::compareWithStarAndQuestionMark(*itExpected, *itActual))
            }
        },
        [&]() -> std::string { return impl::createSideBySideComparison(actual, expected); });
}


/// Convenience macro to call `requireEqualLines()` for the current test instance.
///
/// The macro wraps the check in `WITH_CONTEXT(...)` so the generated side-by-side comparison becomes part of
/// the failure context automatically.
#define REQUIRE_EQUAL_LINES(actual, expected)                                                                          \
    WITH_CONTEXT(erbsland::unittest::th::requireEqualLines(*this, actual, expected));


/// Test if the given string contains valid UTF-8 text.
///
/// This helper validates both the byte-level UTF-8 encoding and the decoded Unicode scalar values. Inputs
/// containing malformed byte sequences, surrogate code-points, or code-points outside the Unicode range fail
/// the current test with a diagnostic that includes the offending byte position.
///
/// @param test The active test instance.
/// @param text The text to validate.
void requireValidUtf8(UnitTest &test, std::string_view text);
/// @overload
/// @param test The active test instance.
/// @param text The UTF-8 text to validate.
void requireValidUtf8(UnitTest &test, std::u8string_view text);


/// Convenience macro to call `requireValidUtf8()` for the current test instance.
#define REQUIRE_VALID_UTF8(str) WITH_CONTEXT(erbsland::unittest::th::requireValidUtf8(*this, str));


}
