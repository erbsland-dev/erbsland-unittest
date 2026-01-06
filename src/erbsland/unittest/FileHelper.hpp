// Copyright © 2025 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include <filesystem>
#include <string>
#include <string_view>
#include <vector>


/// Namespace for file and data helpers.
///
namespace erbsland::unittest::fh {

/// Access the executable path for the unittest executable.
///
/// @return The absolute path to the currently executed unittest executable.
///
auto unitTestExecutablePath() -> std::filesystem::path;

/// Resolves the path to a test data file or directory.
///
/// This method searches for the given relative path in the directory containing the
/// executable and its parent directory to support various CMake build configurations
/// (e.g., MSVC multi-config builds).
///
/// @param relativePath The relative path to the test data.
/// @return The resolved absolute path to the test data.
/// @throws AssertFailed If the path cannot be resolved.
///
auto resolveDataPath(std::string_view relativePath) -> std::filesystem::path;

/// Reads the complete contents of a test data file into a string.
///
/// @param relativePath The relative path to the test data file.
/// @param maximumSize The maximum allowed size of the file in bytes (default 10MB).
/// @return The contents of the file as a string.
/// @throws AssertFailed If the file cannot be found or exceeds the maximum size.
///
auto readDataText(std::string_view relativePath, std::size_t maximumSize = 10000000U) -> std::string;

/// @brief Reads a test data file line by line into a vector of strings.
///
/// @param relativePath The relative path to the test data file.
/// @param maximumSize The maximum allowed size of the file in bytes (default 10MB).
/// @return A vector containing each line of the file.
/// @throws AssertFailed If the file cannot be found or exceeds the maximum size.
///
auto readDataLines(std::string_view relativePath, std::size_t maximumSize = 10000000U) -> std::vector<std::string>;


}
