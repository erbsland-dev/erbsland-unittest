// Copyright (c) 2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0

#include <erbsland/unittest/impl/CommandLine.hpp>
#include <erbsland/unittest/impl/Output.hpp>
#include <erbsland/unittest/UnitTest.hpp>

#include <string>
#include <vector>

using erbsland::unittest::CommandLine;
using erbsland::unittest::Output;

TESTED_TARGETS(CommandLine)
class CommandLineTest final : public el::UnitTest {
private:
    /// A command line and its parsing result.
    struct ParsedCommandLine {
        CommandLine commandLine;      ///< The parsed command line.
        CommandLine::Result result{}; ///< The parsing result.
    };

public:
    void testDefaults() {
        const auto parsed = parse({"self-test"});
        const auto &settings = parsed.commandLine.settings();
        REQUIRE(parsed.result.action == CommandLine::Action::Run);
        REQUIRE_EQUAL(parsed.result.exitCode, 0);
        REQUIRE(settings.verbose == false);
        REQUIRE(settings.stopAtFirstError == false);
        REQUIRE(settings.useColor);
        REQUIRE(settings.showSummary);
        REQUIRE(settings.showTestTime == false);
        REQUIRE(settings.showTimeStatistics == false);
        REQUIRE(settings.filter.isEmpty());
    }

    void testTimingOptionsAreIndependent() {
        const auto shortTiming = parse({"self-test", "-t"});
        REQUIRE(shortTiming.commandLine.settings().showTestTime);
        REQUIRE(shortTiming.commandLine.settings().showTimeStatistics == false);

        const auto longTiming = parse({"self-test", "--time-tests"});
        REQUIRE(longTiming.commandLine.settings().showTestTime);
        REQUIRE(longTiming.commandLine.settings().showTimeStatistics == false);

        const auto statistics = parse({"self-test", "--time-stat"});
        REQUIRE(statistics.commandLine.settings().showTestTime == false);
        REQUIRE(statistics.commandLine.settings().showTimeStatistics);

        const auto combined = parse({"self-test", "-t", "--time-stat"});
        REQUIRE(combined.commandLine.settings().showTestTime);
        REQUIRE(combined.commandLine.settings().showTimeStatistics);
    }

    void testOtherSettings() {
        const auto parsed = parse({"self-test", "-v", "-e", "-c", "-s", "-Xw"});
        const auto &settings = parsed.commandLine.settings();
        REQUIRE(settings.verbose);
        REQUIRE(settings.stopAtFirstError);
        REQUIRE(settings.useColor == false);
        REQUIRE(settings.showSummary == false);
        REQUIRE(settings.waitAfterEachTest);
    }

    void testFilters() {
        const auto parsed =
            parse({"self-test", "name:Alpha", "+name:Beta", "-name:Gamma", "target:Parser", "tag:Fast"});
        const auto &filter = parsed.commandLine.settings().filter;
        REQUIRE(filter.names.exclusive.contains("Alpha"));
        REQUIRE(filter.names.included.contains("Beta"));
        REQUIRE(filter.names.excluded.contains("Gamma"));
        REQUIRE(filter.targets.exclusive.contains("Parser"));
        REQUIRE(filter.tags.exclusive.contains("Fast"));
    }

    void testHelpAndListActions() {
        const auto help = parse({"self-test", "--help"});
        REQUIRE(help.result.action == CommandLine::Action::Exit);
        REQUIRE_EQUAL(help.result.exitCode, 1);

        const auto list = parse({"self-test", "--list"});
        REQUIRE(list.result.action == CommandLine::Action::List);
        REQUIRE_EQUAL(list.result.exitCode, 1);
    }

    void testInvalidArgument() {
        const auto parsed = parse({"self-test", "--no-color", "--unknown"});
        REQUIRE(parsed.result.action == CommandLine::Action::Exit);
        REQUIRE_EQUAL(parsed.result.exitCode, 1);
    }

private:
    static auto parse(const std::vector<std::string> &arguments) -> ParsedCommandLine {
        auto mutableArguments = arguments;
        auto argumentPointers = std::vector<char *>{};
        argumentPointers.reserve(mutableArguments.size());
        for (auto &argument : mutableArguments) {
            argumentPointers.push_back(argument.data());
        }
        auto parsed = ParsedCommandLine{};
        auto output = Output{};
        parsed.result =
            parsed.commandLine.parse(static_cast<int>(argumentPointers.size()), argumentPointers.data(), output);
        return parsed;
    }
};
