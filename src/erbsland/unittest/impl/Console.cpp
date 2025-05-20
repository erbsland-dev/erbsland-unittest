// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "Console.hpp"


#include "MetaData.hpp"

#include <iostream>
#include <utility>


namespace erbsland::unittest {


void Console::setUseColor(bool enabled) {
    _useColor = enabled;
}


void Console::flush() {
    std::cout.flush();
}


void Console::writeLine(const std::string &text) {
    writeLineWithColor(text);
}


void Console::writeDebug(const std::string &text) {
    writeLineWithColor(text, ConsoleColor::DarkGray);
}


void Console::writeError(const std::string &text) {
    writeLineWithColor(text, ConsoleColor::Red);
}


void Console::writeErrorInfo(const std::string &text) {
    writeLineWithColor(text, ConsoleColor::Orange);
}


void Console::writeSuccess(const std::string &text) {
    writeLineWithColor(text, ConsoleColor::Green);
}


void Console::writeLineWithColor(const std::string &text, const ConsoleColor textColor) {
    beforeWriteLine();
    // split multiple lines in `text` to keep things synchronized.
    std::string::size_type lastPos = 0;
    std::string::size_type pos = text.find('\n');
    while (pos != std::string::npos) {
        sendLineSynchronized(ConsoleLine{text.substr(lastPos, pos - lastPos), textColor});
        lastPos = std::exchange(pos, text.find('\n', pos + 1));
    }
    if (const auto lastLine = ConsoleLine{text.substr(lastPos), textColor}; !lastLine.empty()) {
        sendLineSynchronized(lastLine);
    }
    afterWriteLine();
}


auto Console::createTaskLine(
    const TaskInfo &taskInfo,
    const std::string &status,
    const ConsoleColor statusColor) noexcept -> ConsoleLine {

    ConsoleLine line;
    if (status.empty()) {
        line.addText("[", ConsoleColor::LightBlue);
        line.addText(ConsoleLine::rightJustify(taskInfo.taskNumber, 4), ConsoleColor::BrightWhite);
        line.addText("/", ConsoleColor::LightBlue);
        line.addText(ConsoleLine::rightJustify(taskInfo.totalTasks, 4), ConsoleColor::BrightWhite);
        line.addText("] ", ConsoleColor::LightBlue);
        line.addText(taskInfo.text, ConsoleColor::Yellow);
        line.addText(" ...", ConsoleColor::BrightWhite);
    } else {
        line.addText("- ");
        line.addText(taskInfo.text, ConsoleColor::White);
        line.addText(" ");
        line.addText(status, statusColor);
    }
    return line;
}


void Console::startTask(const std::string &text, int taskNumber, int totalTasks) {
    _currentTask.taskNumber = taskNumber;
    _currentTask.totalTasks = totalTasks;
    _currentTask.text = text;
    _currentTaskLine = createTaskLine(_currentTask, {}, {});
    writeTaskLine();
}


void Console::finishTask(const std::string &result, ConsoleColor textColor) {
    clearTaskLine();
    _currentTaskLine = createTaskLine(_currentTask, result, textColor);
    sendLineSynchronized(_currentTaskLine);
    _currentTaskLine = {};
    _currentTask = {};
}


void Console::writeTaskLine() {
    if (!_useColor) {
        return;
    }
    sendLineSynchronized(_currentTaskLine);
    flush();
}


void Console::clearTaskLine() {
    if (_useColor) {
        std::cout << "\x1b[1F\x1b[0K";
    }
}


void Console::beforeWriteLine() {
    if (!_currentTaskLine.empty()) {
        clearTaskLine();
    }
}


void Console::afterWriteLine() {
    if (!_currentTaskLine.empty()) {
        writeTaskLine();
    }
}


void Console::writeTestEntry(const std::string &type, const MetaData &metaData) {
    ConsoleLine result;
    result.addText(type);
    result.addText(": ");
    if (metaData.isSkipByDefault()) {
        result.addText("(");
    }
    result.addText(metaData.shortName(), ConsoleColor::White);
    if (metaData.isSkipByDefault()) {
        result.addText(")");
    }
    if (!metaData.tags().empty()) {
        result.addText(" [", ConsoleColor::DarkCyan);
        bool first = true;
        for (const auto &tag : metaData.tags()) {
            if (!first) {
                result.addText(", ", ConsoleColor::DarkCyan);
            }
            result.addText(tag, ConsoleColor::Cyan);
            first = false;
        }
        result.addText("]", ConsoleColor::DarkCyan);
    }
    if (!metaData.targets().empty()) {
        result.addText(" <", ConsoleColor::Violet);
        bool first = true;
        for (const auto &target : metaData.targets()) {
            if (!first) {
                result.addText(", ", ConsoleColor::Violet);
            }
            result.addText(target, ConsoleColor::Magenta);
            first = false;
        }
        result.addText(">", ConsoleColor::Violet);
    }
    sendLineSynchronized(result);
}


void Console::resetFormatting() {
    if (_useColor) {
        _currentForeground = {};
        _currentBackground = {};
        std::cout << "\x1b[0m\n";
        flush();
    }
}


void Console::writeErrorTaskLine(
    const std::string &task,
    const std::string &result,
    const ConsoleColor textColor) {

    ConsoleLine line;
    line.addText(task);
    line.addText(" ");
    line.addText(result, textColor);
    sendLineSynchronized(line);
}


void Console::sendLineSynchronized(const ConsoleLine &line) noexcept {
    std::unique_lock lock{_mutex};
    for (const auto &part : line.parts()) {
        if (_useColor) {
            if (part.foreground != _currentForeground) {
                _currentForeground = part.foreground;
                std::cout << _currentForeground.foreground();
            }
            if (part.background != _currentBackground) {
                _currentBackground = part.background;
                std::cout << _currentBackground.background();
            }
        }
        std::cout << part.text;
    }
    std::cout << "\n";
    flush();
}


}

