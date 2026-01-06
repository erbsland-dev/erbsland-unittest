// Copyright © 2025 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "FileHelper.hpp"


#include "impl/AssertFailed.hpp"
#include "impl/Console.hpp"
#include "impl/ConsoleColor.hpp"
#include "impl/Controller.hpp"

#include <format>
#include <fstream>


namespace erbsland::unittest::fh {


namespace {

[[noreturn]] void throwError(const std::string &message) {
    auto controller = Controller::instance();
    controller->reportError("FAILED!", ConsoleColor::Red);
    controller->console()->writeError(message);
    throw AssertFailed();
}

}


auto unitTestExecutablePath() -> std::filesystem::path {
    return Controller::instance()->executablePath();
}


auto resolveDataPath(std::string_view relativePath) -> std::filesystem::path {
    const auto exePath = unitTestExecutablePath();
    const auto exeDir = exePath.parent_path();

    auto path1 = exeDir / relativePath;
    if (std::filesystem::exists(path1)) {
        return path1;
    }

    auto path2 = exeDir.parent_path() / relativePath;
    if (std::filesystem::exists(path2)) {
        return path2;
    }

    throwError(std::format(
        "Could not resolve test data path: {}\nTried:\n  {}\n  {}\n",
       relativePath,
       path1.string(),
       path2.string()));
}


auto readDataText(std::string_view relativePath, std::size_t maximumSize) -> std::string {
    const auto path = resolveDataPath(relativePath);

    if (!std::filesystem::is_regular_file(path)) {
        throwError(std::format(
            "Test data path is not a regular file: {}",
            path.string()));
    }

    const auto size = std::filesystem::file_size(path);
    if (size > maximumSize) {
        throwError(std::format(
            "Test data file size ({} bytes) exceeds maximum allowed size ({} bytes): {}",
            size,
            maximumSize,
            path.string()));
    }

    std::ifstream file(path, std::ios::in);
    if (!file) {
        throwError(std::format(
            "Could not open test data file for reading: {}",
            path.string()));
    }

    std::string content;
    content.reserve(static_cast<std::string::size_type>(size));
    content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}


auto readDataLines(std::string_view relativePath, std::size_t maximumSize) -> std::vector<std::string> {
    const auto path = resolveDataPath(relativePath);

    if (!std::filesystem::is_regular_file(path)) {
        throwError(std::format(
            "Test data path is not a regular file: {}",
            path.string()));
    }

    const auto size = std::filesystem::file_size(path);
    if (size > maximumSize) {
        throwError(std::format(
            "Test data file size ({} bytes) exceeds maximum allowed size ({} bytes): {}",
            size,
            maximumSize,
            path.string()));
    }

    std::ifstream file(path, std::ios::in);
    if (!file) {
        throwError(std::format(
            "Could not open test data file for reading: {}",
            path.string()));
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}


}

