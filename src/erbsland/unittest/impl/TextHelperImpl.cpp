// Copyright © 2026 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "TextHelperImpl.hpp"


#include "../TextHelper.hpp"

#include <exception>


namespace erbsland::unittest::th::impl {

constexpr auto cReplacementCharacter = char32_t{0xFFFD};

class Utf8DecodeError final : public std::exception {
public:
    Utf8DecodeError(std::size_t byteIndex, std::string message) : _byteIndex{byteIndex}, _message{std::move(message)} {}

    [[nodiscard]] auto byteIndex() const noexcept -> std::size_t { return _byteIndex; }

    [[nodiscard]] auto message() const noexcept -> const std::string & { return _message; }

    [[nodiscard]] auto what() const noexcept -> const char * override { return _message.c_str(); }

private:
    std::size_t _byteIndex;
    std::string _message;
};

auto hexDigitValue(const char digit) -> int {
    if (digit >= '0' && digit <= '9') {
        return digit - '0';
    }
    if (digit >= 'a' && digit <= 'f') {
        return 10 + (digit - 'a');
    }
    if (digit >= 'A' && digit <= 'F') {
        return 10 + (digit - 'A');
    }
    return -1;
}

auto appendUtf8FromCodePoint(std::string &result, char32_t codePoint) -> void {
    if ((codePoint >= 0xD800 && codePoint <= 0xDFFF) || codePoint > 0x10FFFF) {
        codePoint = cReplacementCharacter;
    }
    if (codePoint < 0x80) {
        result += static_cast<char>(codePoint);
    } else if (codePoint < 0x800) {
        result += static_cast<char>(0xC0 | ((codePoint >> 6) & 0x1F));
        result += static_cast<char>(0x80 | (codePoint & 0x3F));
    } else if (codePoint < 0x10000) {
        result += static_cast<char>(0xE0 | ((codePoint >> 12) & 0x0F));
        result += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (codePoint & 0x3F));
    } else {
        result += static_cast<char>(0xF0 | ((codePoint >> 18) & 0x07));
        result += static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F));
        result += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (codePoint & 0x3F));
    }
}

auto decodeUtf8CodePoint(const std::string_view text, std::size_t &byteIndex) -> char32_t {
    const auto leadByte = static_cast<uint8_t>(text.at(byteIndex));
    if (leadByte < 0x80U) {
        ++byteIndex;
        return leadByte;
    }
    if (leadByte < 0xC0U) {
        throw Utf8DecodeError{byteIndex, "unexpected continuation byte"};
    }
    if (leadByte < 0xC2U) {
        throw Utf8DecodeError{byteIndex, "overlong UTF-8 start byte"};
    }
    if (leadByte < 0xE0U) {
        const auto byteCount = std::size_t{2};
        if (byteIndex + byteCount > text.size()) {
            throw Utf8DecodeError{byteIndex, "truncated 2-byte UTF-8 sequence"};
        }
        const auto byte1 = static_cast<uint8_t>(text.at(byteIndex + 1));
        if ((byte1 & 0xC0U) != 0x80U) {
            throw Utf8DecodeError{byteIndex, "invalid continuation byte in 2-byte UTF-8 sequence"};
        }
        byteIndex += byteCount;
        return (static_cast<char32_t>(leadByte & 0x1FU) << 6) | static_cast<char32_t>(byte1 & 0x3FU);
    }
    if (leadByte < 0xF0U) {
        const auto byteCount = std::size_t{3};
        if (byteIndex + byteCount > text.size()) {
            throw Utf8DecodeError{byteIndex, "truncated 3-byte UTF-8 sequence"};
        }
        const auto byte1 = static_cast<uint8_t>(text.at(byteIndex + 1));
        const auto byte2 = static_cast<uint8_t>(text.at(byteIndex + 2));
        if ((byte1 & 0xC0U) != 0x80U || (byte2 & 0xC0U) != 0x80U) {
            throw Utf8DecodeError{byteIndex, "invalid continuation byte in 3-byte UTF-8 sequence"};
        }
        if (leadByte == 0xE0U && byte1 < 0xA0U) {
            throw Utf8DecodeError{byteIndex, "overlong 3-byte UTF-8 sequence"};
        }
        if (leadByte == 0xEDU && byte1 >= 0xA0U) {
            throw Utf8DecodeError{byteIndex, "UTF-8 sequence encodes a surrogate code point"};
        }
        byteIndex += byteCount;
        return (static_cast<char32_t>(leadByte & 0x0FU) << 12) | (static_cast<char32_t>(byte1 & 0x3FU) << 6) |
            static_cast<char32_t>(byte2 & 0x3FU);
    }
    if (leadByte < 0xF5U) {
        const auto byteCount = std::size_t{4};
        if (byteIndex + byteCount > text.size()) {
            throw Utf8DecodeError{byteIndex, "truncated 4-byte UTF-8 sequence"};
        }
        const auto byte1 = static_cast<uint8_t>(text.at(byteIndex + 1));
        const auto byte2 = static_cast<uint8_t>(text.at(byteIndex + 2));
        const auto byte3 = static_cast<uint8_t>(text.at(byteIndex + 3));
        if ((byte1 & 0xC0U) != 0x80U || (byte2 & 0xC0U) != 0x80U || (byte3 & 0xC0U) != 0x80U) {
            throw Utf8DecodeError{byteIndex, "invalid continuation byte in 4-byte UTF-8 sequence"};
        }
        if (leadByte == 0xF0U && byte1 < 0x90U) {
            throw Utf8DecodeError{byteIndex, "overlong 4-byte UTF-8 sequence"};
        }
        if (leadByte == 0xF4U && byte1 >= 0x90U) {
            throw Utf8DecodeError{byteIndex, "UTF-8 sequence encodes a code point beyond U+10FFFF"};
        }
        byteIndex += byteCount;
        return (static_cast<char32_t>(leadByte & 0x07U) << 18) | (static_cast<char32_t>(byte1 & 0x3FU) << 12) |
            (static_cast<char32_t>(byte2 & 0x3FU) << 6) | static_cast<char32_t>(byte3 & 0x3FU);
    }
    throw Utf8DecodeError{byteIndex, "invalid UTF-8 start byte"};
}

auto toStdString(const std::string_view str) -> std::string {
    return std::string{str};
}

auto toStdString(const std::u8string_view str) -> std::string {
    return std::string{reinterpret_cast<const char *>(str.data()), str.size()};
}

auto toStdString(const std::u16string_view str) -> std::string {
    std::string result;
    result.reserve(str.size() * 3); // UTF-8 can use up to 3 bytes per UTF-16 code unit (4 bytes for surrogate pairs)
    for (std::size_t i = 0; i < str.size(); ++i) {
        const char16_t unit = str[i];
        // Check for high surrogate (0xD800-0xDBFF)
        if (unit >= 0xD800 && unit <= 0xDBFF) {
            // Need a low surrogate to follow
            if (i + 1 < str.size()) {
                const char16_t low = str[i + 1];
                if (low >= 0xDC00 && low <= 0xDFFF) {
                    // Valid surrogate pair - decode to code point
                    const char32_t codePoint = 0x10000 + ((unit - 0xD800) << 10) + (low - 0xDC00);
                    appendUtf8FromCodePoint(result, codePoint);
                    ++i; // Skip the low surrogate
                    continue;
                }
            }
            // Invalid surrogate - use replacement character
            appendUtf8FromCodePoint(result, cReplacementCharacter);
        }
        // Check for low surrogate without high surrogate (0xDC00-0xDFFF)
        else if (unit >= 0xDC00 && unit <= 0xDFFF) {
            // Invalid - use replacement character
            appendUtf8FromCodePoint(result, cReplacementCharacter);
        } else {
            appendUtf8FromCodePoint(result, static_cast<char32_t>(unit));
        }
    }

    return result;
}

auto toStdString(std::u32string_view str) -> std::string {
    std::string result;
    result.reserve(str.size() * 4); // UTF-8 can use up to 4 bytes per code point
    for (const auto codePoint : str) {
        appendUtf8FromCodePoint(result, codePoint);
    }

    return result;
}

auto toStdString(std::wstring_view str) -> std::string {
    if constexpr (sizeof(wchar_t) == 2) {
        return toStdString({reinterpret_cast<const char16_t *>(str.data()), str.size()});
    } else {
        return toStdString({reinterpret_cast<const char32_t *>(str.data()), str.size()});
    }
}

auto toStdU32String(std::string_view str) -> std::u32string {
    auto result = std::u32string{};
    result.reserve(str.size());
    for (std::size_t byteIndex = 0; byteIndex < str.size();) {
        try {
            result.push_back(decodeUtf8CodePoint(str, byteIndex));
        } catch (const Utf8DecodeError &) {
            result.push_back(cReplacementCharacter);
            ++byteIndex;
        }
    }
    return result;
}

auto toStdU32String(std::u8string_view str) -> std::u32string {
    return toStdU32String({reinterpret_cast<const char *>(str.data()), str.size()});
}

auto toStdU32String(std::u16string_view str) -> std::u32string {
    auto result = std::u32string{};
    result.reserve(str.size());
    for (std::size_t i = 0; i < str.size(); ++i) {
        const auto unit = str[i];
        if (unit >= 0xD800 && unit <= 0xDBFF) {
            if (i + 1 < str.size()) {
                const auto low = str[i + 1];
                if (low >= 0xDC00 && low <= 0xDFFF) {
                    const auto codePoint =
                        0x10000U + (static_cast<char32_t>(unit - 0xD800) << 10) + static_cast<char32_t>(low - 0xDC00);
                    result.push_back(codePoint);
                    ++i;
                    continue;
                }
            }
            result.push_back(cReplacementCharacter);
            continue;
        }
        if (unit >= 0xDC00 && unit <= 0xDFFF) {
            result.push_back(cReplacementCharacter);
            continue;
        }
        result.push_back(static_cast<char32_t>(unit));
    }
    return result;
}

auto toStdU32String(std::u32string_view str) -> std::u32string {
    return std::u32string{str};
}

auto toStdU32String(std::wstring_view str) -> std::u32string {
    if constexpr (sizeof(wchar_t) == 2) {
        return toStdU32String({reinterpret_cast<const char16_t *>(str.data()), str.size()});
    } else {
        return toStdU32String({reinterpret_cast<const char32_t *>(str.data()), str.size()});
    }
}

auto validateUtf8(std::string_view str) -> Utf8ValidationResult {
    for (std::size_t byteIndex = 0; byteIndex < str.size();) {
        try {
            static_cast<void>(decodeUtf8CodePoint(str, byteIndex));
        } catch (const Utf8DecodeError &error) {
            return Utf8ValidationResult{false, error.byteIndex(), error.message()};
        }
    }
    return Utf8ValidationResult{true};
}

auto validateUtf8(std::u8string_view str) -> Utf8ValidationResult {
    return validateUtf8({reinterpret_cast<const char *>(str.data()), str.size()});
}

auto characterCount(const std::string_view str) noexcept -> std::size_t {
    return ConsoleLine::utf8Length(str);
}

auto characterCount(const std::u8string_view str) noexcept -> std::size_t {
    return ConsoleLine::utf8Length({reinterpret_cast<const char *>(str.data()), str.size()});
}

auto characterCount(std::u16string_view str) noexcept -> std::size_t {
    std::size_t result = 0;
    for (std::size_t i = 0; i < str.size(); ++i) {
        // Check for high surrogate (0xD800-0xDBFF)
        if (str[i] >= 0xD800 && str[i] <= 0xDBFF) {
            // Need a low surrogate to follow
            if (i + 1 < str.size()) {
                if (str[i + 1] >= 0xDC00 && str[i + 1] <= 0xDFFF) {
                    result += 1;
                    ++i; // Skip the low surrogate
                    continue;
                }
            }
        }
        result += 1;
    }
    return result;
}

auto characterCount(std::u32string_view str) noexcept -> std::size_t {
    return str.size();
}

auto characterCount(std::wstring_view str) noexcept -> std::size_t {
    if constexpr (sizeof(wchar_t) == 2) {
        return characterCount({reinterpret_cast<const char16_t *>(str.data()), str.size()});
    } else {
        return characterCount({reinterpret_cast<const char32_t *>(str.data()), str.size()});
    }
}

}
