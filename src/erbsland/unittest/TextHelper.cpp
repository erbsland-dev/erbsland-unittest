// Copyright © 2026 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "TextHelper.hpp"

#include "impl/ConsoleLine.hpp"


namespace erbsland::unittest::th {


auto characterCount(const std::string_view text) noexcept -> std::size_t {
    return impl::characterCount(text);
}

auto characterCount(const std::u8string_view text) noexcept -> std::size_t {
    return impl::characterCount(text);
}

auto characterCount(const std::u16string_view text) noexcept -> std::size_t {
    return impl::characterCount(text);
}

auto characterCount(const std::u32string_view text) noexcept -> std::size_t {
    return impl::characterCount(text);
}

auto characterCount(const std::wstring_view text) noexcept -> std::size_t {
    return impl::characterCount(text);
}

auto toConsoleSafeString(const std::string_view text, const std::size_t maxLength) noexcept -> std::string {
    return ConsoleLine::utf8SafeString(text, maxLength);
}

auto toConsoleSafeString(const std::u8string_view text, const std::size_t maxLength) noexcept -> std::string {
    return ConsoleLine::utf8SafeString(impl::toStdString(text), maxLength);
}

auto toConsoleSafeString(const std::u16string_view text, const std::size_t maxLength) noexcept -> std::string {
    return ConsoleLine::utf8SafeString(impl::toStdString(text), maxLength);
}

auto toConsoleSafeString(const std::u32string_view text, const std::size_t maxLength) noexcept -> std::string {
    return ConsoleLine::utf8SafeString(impl::toStdString(text), maxLength);
}

auto toConsoleSafeString(const std::wstring_view text, const std::size_t maxLength) noexcept -> std::string {
    return ConsoleLine::utf8SafeString(impl::toStdString(text), maxLength);
}

auto toStdString(std::string_view text) -> std::string {
    return impl::toStdString(text);
}

auto toStdString(std::u8string_view text) -> std::string {
    return impl::toStdString(text);
}

auto toStdString(std::u16string_view text) -> std::string {
    return impl::toStdString(text);
}

auto toStdString(std::u32string_view text) -> std::string {
    return impl::toStdString(text);
}

auto toStdString(std::wstring_view text) -> std::string {
    return impl::toStdString(text);
}

auto toStdU32String(const std::string_view text) -> std::u32string {
    return impl::toStdU32String(text);
}

auto toStdU32String(const std::u8string_view text) -> std::u32string {
    return impl::toStdU32String(text);
}

auto toStdU32String(const std::u16string_view text) -> std::u32string {
    return impl::toStdU32String(text);
}

auto toStdU32String(const std::u32string_view text) -> std::u32string {
    return impl::toStdU32String(text);
}

auto toStdU32String(const std::wstring_view text) -> std::u32string {
    return impl::toStdU32String(text);
}

auto stdStringFromHex(const std::string_view hexString) -> std::string {
    return impl::stringFromHex<std::string>(hexString, 2);
}

auto stdU8StringFromHex(const std::string_view hexString) -> std::u8string {
    return impl::stringFromHex<std::u8string>(hexString, 2);
}

auto stdU16StringFromHex(const std::string_view hexString) -> std::u16string {
    return impl::stringFromHex<std::u16string>(hexString, 4);
}

auto stdU32StringFromHex(const std::string_view hexString) -> std::u32string {
    return impl::stringFromHex<std::u32string>(hexString, 8);
}

auto stdWStringFromHex(const std::string_view hexString) -> std::wstring {
    return impl::stringFromHex<std::wstring>(hexString, sizeof(wchar_t) * 2U);
}

auto appendInvalidUtf8Sequence(std::string &result, const Utf8Error error) -> void {
    switch (error) {
    case Utf8Error::UnexpectedContinuationByte:
        result.push_back(static_cast<char>(0x80));
        return;
    case Utf8Error::Overlong2ByteSequence:
        result.push_back(static_cast<char>(0xC0));
        result.push_back(static_cast<char>(0x80));
        return;
    case Utf8Error::Truncated2ByteSequence:
        result.push_back(static_cast<char>(0xC2));
        return;
    case Utf8Error::InvalidContinuationByteIn2ByteSequence:
        result.push_back(static_cast<char>(0xC2));
        result.push_back(static_cast<char>(0x20));
        return;
    case Utf8Error::Overlong3ByteSequence:
        result.push_back(static_cast<char>(0xE0));
        result.push_back(static_cast<char>(0x80));
        result.push_back(static_cast<char>(0x80));
        return;
    case Utf8Error::Truncated3ByteSequence:
        result.push_back(static_cast<char>(0xE1));
        result.push_back(static_cast<char>(0x80));
        return;
    case Utf8Error::InvalidContinuationByteIn3ByteSequence:
        result.push_back(static_cast<char>(0xE1));
        result.push_back(static_cast<char>(0x80));
        result.push_back(static_cast<char>(0x20));
        return;
    case Utf8Error::SurrogateCodePoint:
        result.push_back(static_cast<char>(0xED));
        result.push_back(static_cast<char>(0xA0));
        result.push_back(static_cast<char>(0x80));
        return;
    case Utf8Error::Overlong4ByteSequence:
        result.push_back(static_cast<char>(0xF0));
        result.push_back(static_cast<char>(0x80));
        result.push_back(static_cast<char>(0x80));
        result.push_back(static_cast<char>(0x80));
        return;
    case Utf8Error::Truncated4ByteSequence:
        result.push_back(static_cast<char>(0xF1));
        result.push_back(static_cast<char>(0x80));
        result.push_back(static_cast<char>(0x80));
        return;
    case Utf8Error::InvalidContinuationByteIn4ByteSequence:
        result.push_back(static_cast<char>(0xF1));
        result.push_back(static_cast<char>(0x80));
        result.push_back(static_cast<char>(0x80));
        result.push_back(static_cast<char>(0x20));
        return;
    case Utf8Error::CodePointBeyondUnicodeRange:
        result.push_back(static_cast<char>(0xF4));
        result.push_back(static_cast<char>(0x90));
        result.push_back(static_cast<char>(0x80));
        result.push_back(static_cast<char>(0x80));
        return;
    case Utf8Error::InvalidStartByte:
        result.push_back(static_cast<char>(0xF8));
        return;
    default:
        break;
    }
}

auto invalidUtf8(
    const Utf8Error error, const std::optional<std::string_view> &prefix, const std::optional<std::string_view> &suffix)
    -> std::string {

    auto result = std::string{};
    result.reserve((prefix ? prefix->size() : 0U) + 4U + (suffix ? suffix->size() : 0U));
    if (prefix) {
        result += *prefix;
    }
    appendInvalidUtf8Sequence(result, error);
    if (suffix) {
        result += *suffix;
    }
    return result;
}

auto splitLineViews(const std::string_view text) -> std::vector<std::string_view> {
    return impl::splitLineViews(text);
}

auto splitLineViews(const std::u8string_view text) -> std::vector<std::u8string_view> {
    return impl::splitLineViews(text);
}

auto splitLineViews(const std::u16string_view text) -> std::vector<std::u16string_view> {
    return impl::splitLineViews(text);
}

auto splitLineViews(const std::u32string_view text) -> std::vector<std::u32string_view> {
    return impl::splitLineViews(text);
}

auto splitLineViews(const std::wstring_view text) -> std::vector<std::wstring_view> {
    return impl::splitLineViews(text);
}

auto splitLines(const std::string_view text) -> std::vector<std::string> {
    return impl::splitLines<std::string_view, std::string>(text);
}

auto splitLines(const std::u8string_view text) -> std::vector<std::u8string> {
    return impl::splitLines<std::u8string_view, std::u8string>(text);
}

auto splitLines(const std::u16string_view text) -> std::vector<std::u16string> {
    return impl::splitLines<std::u16string_view, std::u16string>(text);
}

auto splitLines(const std::u32string_view text) -> std::vector<std::u32string> {
    return impl::splitLines<std::u32string_view, std::u32string>(text);
}

auto splitLines(const std::wstring_view text) -> std::vector<std::wstring> {
    return impl::splitLines<std::wstring_view, std::wstring>(text);
}

void requireValidUtf8(UnitTest &test, const std::string_view text) {
    const auto validation = impl::validateUtf8(text);
    test.runWithContext(
        SOURCE_LOCATION(),
        [&]() -> void { ASSERT_CONTEXT_REQUIRE_FOR_TEST(&test, "REQUIRE_VALID_UTF8", 0, validation.valid) },
        [&]() -> std::string {
            return std::format(
                "Invalid UTF-8 at byte {}: {}\n  Text: {}",
                validation.byteIndex,
                validation.message,
                ConsoleLine::utf8SafeString(text, 120U));
        });
}

void requireValidUtf8(UnitTest &test, const std::u8string_view text) {
    const auto validation = impl::validateUtf8(text);
    test.runWithContext(
        SOURCE_LOCATION(),
        [&]() -> void { ASSERT_CONTEXT_REQUIRE_FOR_TEST(&test, "REQUIRE_VALID_UTF8", 0, validation.valid) },
        [&]() -> std::string {
            return std::format(
                "Invalid UTF-8 at byte {}: {}\n  Text: {}",
                validation.byteIndex,
                validation.message,
                ConsoleLine::utf8SafeString(impl::toStdString(text), 120U));
        });
}


}
