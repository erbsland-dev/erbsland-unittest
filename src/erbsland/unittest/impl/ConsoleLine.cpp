// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "ConsoleLine.hpp"


#include <algorithm>
#include <format>
#include <numeric>


namespace erbsland::unittest {


ConsoleLine::ConsoleLine(const std::string &text, const ConsoleColor foreground, const ConsoleColor background) noexcept
    : _parts{1, Part{utf8RemoveControlCharacters(text), foreground, background}} {
}


auto ConsoleLine::empty() const noexcept -> bool {
    return length() == 0;
}


auto ConsoleLine::length() const noexcept -> std::size_t {
    return std::accumulate(_parts.begin(), _parts.end(), std::size_t{0}, [](auto sum, const auto &part) {
        return sum + utf8Length(part.text);
    });
}


auto ConsoleLine::parts() const noexcept -> const std::vector<Part>& {
    return _parts;
}


void ConsoleLine::addText(
    const std::string &text,
    const ConsoleColor foreground,
    const ConsoleColor background) noexcept {

    _parts.emplace_back(Part{utf8RemoveControlCharacters(text), foreground, background});
}


auto ConsoleLine::utf8Length(const std::string_view &text) noexcept -> std::size_t {
    return std::count_if(text.begin(), text.end(), [](unsigned char character) -> bool {
        return (character & 0xC0) != 0x80;
    });
}


auto ConsoleLine::utf8RemoveControlCharacters(const std::string_view &text) noexcept -> std::string {
    std::string result;
    result.reserve(text.size());
    for (const auto character : text) {
        const auto asByte = static_cast<uint8_t>(character);
        if (asByte < 0x20U || asByte == 0x7FU) {
            continue;
        }
        result.push_back(character);
    }
    return result;
}


/// Processes a potentially valid UTF-8 encoded character from the input text, appending a representation of the
/// character to the result string. This function ensures that the UTF-8 data is safe and properly encoded.
/// Invalid or incomplete data will be represented by an escaped hexadecimal notation.
///
/// @param currentIndex Reference to the current index in the text to be processed.
///     Will be updated to point to the next character index after processing multibyte sequences.
/// @param text The string view containing the UTF-8 encoded text to process.
/// @param result The string to which the processed and validated character representation will be appended.
///
void processSafeUtf8Character(
    std::size_t &currentIndex,
    const std::string_view &text,
    std::string &result) {

    const auto asByte = static_cast<uint8_t>(text.at(currentIndex));
    std::size_t cSize = 0;
    char32_t unicodeValue{};
    if ((asByte & 0b11100000U) == 0b11000000U && asByte >= 0b11000010U) {
        cSize = 2; // 2-byte sequence
        unicodeValue = static_cast<char32_t>(asByte & 0b00011111U);
    } else if ((asByte & 0b11110000U) == 0b11100000U) {
        cSize = 3; // 3-byte sequence
        unicodeValue = static_cast<char32_t>(asByte & 0b00001111U);
    } else if ((asByte & 0b11111000U) == 0b11110000U && asByte < 0b11110101U) {
        cSize = 4; // 4-byte sequence
        unicodeValue = static_cast<char32_t>(asByte & 0b00000111U);
    }
    if (cSize < 2) {
        // invalid size or wrong state.
        result.append(std::format("\\x{:02X}", asByte));
    } else if (currentIndex + cSize > text.size()) {
        // the text is too short.
        result.append(std::format("\\x{:02X}", asByte));
    } else {
        bool valid = true;
        for (std::size_t di = 1; di < cSize; ++di) {
            const auto nextByte = static_cast<uint8_t>(text.at(currentIndex + di));
            if ((nextByte & 0b11000000U) != 0b10000000U) {
                // invalid UTF-8 encoding.
                result.append(std::format("\\x{:02X}", asByte));
                valid = false;
                break;
            }
            unicodeValue <<= 6;
            unicodeValue |= static_cast<char32_t>(nextByte & 0b00111111U);
        }
        if (valid) {
            result.append(std::format("\\u{{{:04X}}}", static_cast<uint32_t>(unicodeValue)));
            currentIndex += cSize - 1;
        }
    }
}


/// Processes a single character from the input text, appending a representation of the character to the result string.
/// This function ensures that the character is safe and properly encoded.
///
/// @param currentIndex Reference to the current index in the text to be processed.
///     Will be updated to point to the next character index after processing multibyte sequences.
/// @param text The string view containing the UTF-8 encoded text to process.
/// @param result The string to which the processed and validated character representation will be appended.
///
void processCharacterForSafeUtf8Encoding(
    std::size_t &currentIndex,
    const std::string_view &text,
    std::string &result) {

    const auto character = text.at(currentIndex);
    switch (character) {
    case '\n': result.append("\\n"); return;
    case '\r': result.append("\\r"); return;
    case '\t': result.append("\\t"); return;
    case '\\': result.append("\\\\"); return;
    case '\"': result.append("\\\""); return;
    default: break;
    }

    const auto asByte = static_cast<uint8_t>(character);
    if (asByte < 0x20U || asByte == 0x7FU) {
        result.append(std::format("\\x{:02X}", asByte));
    } else if (asByte < 0x80U) {
        result.push_back(character);
    } else {
        processSafeUtf8Character(currentIndex, text, result);
    }
}


auto ConsoleLine::utf8SafeString(const std::string_view &text, const std::size_t maxLength) noexcept -> std::string {
    std::string result;
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (i >= maxLength) {
            result.append(std::format("(... +{} more)", text.size() - maxLength));
            break;
        }
        processCharacterForSafeUtf8Encoding(i, text, result);
    }
    // If the string contains
    if (result.find_first_of(" \"\\") != std::string::npos) {
        result.insert(0, "\"");
        result.push_back('\"');
    }
    return result;
}


}

