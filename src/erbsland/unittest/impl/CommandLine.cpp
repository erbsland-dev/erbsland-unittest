// Copyright (c) 2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#include "CommandLine.hpp"

#include "Output.hpp"

#include <sstream>
#include <string>

namespace erbsland::unittest {

auto CommandLine::parse(const int argc, char **argv, Output &output) -> Result {
    _settings = {};
    output.setUseColor(true);
    if (argc > 0) {
        _settings.executablePath = std::filesystem::path{argv[0]};
        try {
            _settings.executablePath = std::filesystem::absolute(_settings.executablePath);
        } catch (const std::filesystem::filesystem_error &) {
            // Keep the original path if it cannot be made absolute.
        }
    }

    for (int index = 1; index < argc; ++index) {
        const auto argument = std::string{argv[index]};
        if (argument == "-h" || argument == "-help" || argument == "--help") {
            printHelp(output);
            return {Action::Exit, 1};
        }
        if (argument == "-l" || argument == "--list") {
            return {Action::List, 1};
        }
        if (argument == "-v" || argument == "--verbose") {
            _settings.verbose = true;
        } else if (argument == "-e") {
            _settings.stopAtFirstError = true;
        } else if (argument == "-c" || argument == "--no-color") {
            _settings.useColor = false;
            output.setUseColor(false);
        } else if (argument == "-s" || argument == "--no-summary") {
            _settings.showSummary = false;
        } else if (argument == "-t" || argument == "--time-tests") {
            _settings.showTestTime = true;
        } else if (argument == "--time-stat") {
            _settings.showTimeStatistics = true;
        } else if (argument == "-Xw") {
            _settings.waitAfterEachTest = true;
        } else if (!addFilter(argument)) {
            output.writeError("Unknown command line argument \"" + argument + "\"\n");
            printHelp(output);
            return {Action::Exit, 1};
        }
    }
    return {Action::Run, 0};
}

auto CommandLine::settings() const noexcept -> const Settings & {
    return _settings;
}

auto CommandLine::addFilter(const std::string &argument) -> bool {
    const auto separator = argument.find(':');
    if (separator == 0 || separator == std::string::npos) {
        return false;
    }

    auto option = argument.substr(0, separator);
    const auto value = argument.substr(separator + 1);
    auto type = FilterOption::Exclusive;
    if (option.front() == '+') {
        type = FilterOption::Included;
        option = option.substr(1);
    } else if (option.front() == '-') {
        type = FilterOption::Excluded;
        option = option.substr(1);
    }

    FilterRule *rule = nullptr;
    if (option == "name") {
        rule = &_settings.filter.names;
    } else if (option == "target") {
        rule = &_settings.filter.targets;
    } else if (option == "tag") {
        rule = &_settings.filter.tags;
    } else {
        return false;
    }

    switch (type) {
    case FilterOption::Exclusive:
        rule->exclusive.insert(value);
        break;
    case FilterOption::Included:
        rule->included.insert(value);
        break;
    case FilterOption::Excluded:
        rule->excluded.insert(value);
        break;
    }
    return true;
}

void CommandLine::printHelp(Output &output) const {
    auto text = std::stringstream{};
    text << "Erbsland Unit Test Help:\n"
         << "  -h/--help ......... Display this help\n"
         << "  -v/--verbose ...... Display verbose messages. Skipped tests.\n"
         << "  -e ................ Stop at the first error.\n"
         << "  -l/--list ......... List all suites and tests. Do not run any test.\n"
         << "  -c/--no-color ..... Do not colorize the output and disable status updates.\n"
         << "  -s/--no-summary ... Do not list the first three errors at the end of the run.\n"
         << "  -t/--time-tests ... Display the runtime of each test and suite.\n"
         << "  --time-stat ....... Display time statistics after the test result.\n"
         << "  name:<name> ....... Exclusively run tests with the specified test or class name (case sensitive).\n"
         << "  +name:<name> ...... Run tests with the specified test or class name, even optional ones.\n"
         << "  -name:<name> ...... Skip tests with the specified test or class name.\n"
         << "  target:<target> ... Exclusively run tests for the specified target.\n"
         << "  +target:<target> .. Run tests with for the specified target, even optional ones.\n"
         << "  -target:<target> .. Skip tests with for the specified target.\n"
         << "  tag:<tag> ......... Exclusively run tests with the specified tags.\n"
         << "  +tag:<tag> ........ Run tests with the specified tags, even optional ones.\n"
         << "  -tag:<tag> ........ Skip tests with the specified tags.\n"
         << "\n"
         << "By default all tests that are not marked with `SKIP_BY_DEFAULT()` are enabled.\n"
         << "You can individually add `+` or remove `-` tests from this initial set.\n"
         << "\n"
         << "If you specify one or more options like `<opt>:<tag>`, only tests with \n"
         << "are enabled, and further `+/-` options will change this set.\n"
         << "\n"
         << "The processing order of the options is <opt>, +<opt>, -<opt> and does not depend\n"
         << "on the order how they are specified on the command line. Therefore `-` always have\n"
         << "the highest priority and will skip these tests no mather what was specified otherwise.\n";
    output.writeLine(text.str());
}

}
