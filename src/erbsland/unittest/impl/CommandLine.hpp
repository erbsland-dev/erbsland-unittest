// Copyright (c) 2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "Filter.hpp"

#include <filesystem>
#include <string>

namespace erbsland::unittest {

class Output;

/// @internal
/// Parse and store all command-line settings for a test run.
/// @tested{CommandLineTest}
class CommandLine final {
public:
    /// The action selected by command-line processing.
    /// @tested{CommandLineTest}
    enum class Action {
        Run,  ///< Run the selected tests.
        List, ///< List all registered tests.
        Exit, ///< Exit after displaying help or an error.
    };

    /// The result of command-line processing.
    /// @tested{CommandLineTest}
    struct Result {
        Action action{Action::Run}; ///< The selected action.
        int exitCode{};             ///< The exit code for non-run actions.
    };

    /// All settings parsed from the command line.
    /// @tested{CommandLineTest}
    struct Settings {
        std::filesystem::path executablePath; ///< The absolute path of the test executable.
        Filter filter;                        ///< The test selection filter.
        bool verbose{false};                  ///< Display skipped tests.
        bool stopAtFirstError{false};         ///< Stop after the first failed test.
        bool useColor{true};                  ///< Enable colored output and status updates.
        bool showSummary{true};               ///< Display the first errors after a failed run.
        bool waitAfterEachTest{false};        ///< Wait one second after every test.
        bool showTestTime{false};             ///< Display method and suite timings.
        bool showTimeStatistics{false};       ///< Display timing statistics.
    };

public:
    /// Parse all command-line arguments.
    /// @param argc The number of arguments.
    /// @param argv The argument values.
    /// @param output The output used for help and errors.
    /// @return The selected action and exit code.
    [[nodiscard]] auto parse(int argc, char *argv[], Output &output) -> Result;
    /// Access the parsed settings.
    [[nodiscard]] auto settings() const noexcept -> const Settings &;
    /// Display the command-line help.
    /// @param output The output used to display the help.
    void printHelp(Output &output) const;

private:
    /// Add a filter option.
    /// @param argument The complete filter argument.
    /// @return True if the filter option was recognized.
    [[nodiscard]] auto addFilter(const std::string &argument) -> bool;

private:
    Settings _settings{}; ///< The settings for the current invocation.
};

}
