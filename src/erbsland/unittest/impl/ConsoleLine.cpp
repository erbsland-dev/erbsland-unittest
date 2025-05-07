// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "ConsoleLine.hpp"


#include <algorithm>
#include <format>
#include <numeric>
#include <__filesystem/file_type.h>


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


auto ConsoleLine::utf8SafeString(const std::string_view &text, std::size_t maxLength) noexcept -> std::string {
    std::string result;
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (i >= maxLength) {
            result.append(std::format("(... +{} more)", text.size() - maxLength));
            break;
        }
        const auto character = text[i];
        auto asByte = static_cast<uint8_t>(character);
        if (asByte < 0x20U || asByte == 0x7FU) {
            if (character == '\n') {
                result.append("\\n");
            } else if (character == '\r') {
                result.append("\\r");
            } else if (character == '\t') {
                result.append("\\t");
            } else if (character == '\\') {
                result.append("\\\\");
            } else if (character == '"') {
                result.append("\\\"");
            } else {
                result.append(std::format("\\x{:02X}", asByte));
            }
        } else if (asByte < 0x80U) {
            result.push_back(character);
        } else {
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
            } else if (i + cSize > text.size()) {
                // the text is too short.
                result.append(std::format("\\x{:02X}", asByte));
            } else {
                bool valid = true;
                for (std::size_t di = 1; di < cSize; ++di) {
                    const auto nextByte = static_cast<uint8_t>(text[i + di]);
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
                    i += cSize - 1;
                }
            }
        }
    }
    // If the string contains
    if (result.find_first_of(" \"\\") != std::string::npos) {
        result.insert(0, "\"");
        result.push_back('\"');
    }
    return result;
}


}

