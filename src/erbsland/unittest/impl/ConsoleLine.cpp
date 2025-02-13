// Copyright © 2025 EducateIT GmbH (https://educateit.ch/) and Tobias Erbsland (https://erbsland.dev/)
// All rights are reserved. Detailed terms are specified in the "COPYRIGHT.md" file.
#include "ConsoleLine.hpp"


#include <algorithm>
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
    for (auto character : text) {
        if (character < 32 || character == 127) {
            continue;
        }
        result.push_back(character);
    }
    return result;
}


}

