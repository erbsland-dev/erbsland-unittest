// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "Console.hpp"


#include "MetaData.hpp"

#include <iostream>
#include <sstream>


namespace erbsland::unittest {


Console::Console() {
    _colorMap = {
        {Black,       std::make_tuple(30, 40)},
        {DarkRed,     std::make_tuple(31, 41)},
        {Green,       std::make_tuple(32, 42)},
        {Orange,      std::make_tuple(33, 43)},
        {DarkBlue,    std::make_tuple(34, 44)},
        {Violet,      std::make_tuple(35, 45)},
        {DarkCyan,    std::make_tuple(36, 46)},
        {White,       std::make_tuple(37, 47)},
        {DarkGray,    std::make_tuple(90, 90)},
        {Red,         std::make_tuple(91, 101)},
        {LightGreen,  std::make_tuple(92, 102)},
        {Yellow,      std::make_tuple(93, 103)},
        {LightBlue,   std::make_tuple(94, 104)},
        {Magenta,     std::make_tuple(95, 105)},
        {Cyan,        std::make_tuple(96, 106)},
        {BrightWhite, std::make_tuple(97, 107)},
    };
}


void Console::setUseColor(bool enabled) {
    _useColor = enabled;
}


void Console::write(const std::string &text, Console::Color textColor) {
    if (_useColor) {
        if (textColor != Default) {
            std::cout << "\x1b[" << std::get<0>(_colorMap[textColor]) << "m";
        }
    }
    std::cout << text;
    if (_useColor) {
        if (textColor != Default) {
            std::cout << "\x1b[0m";
        }
    }
}


void Console::flush() {
    std::cout.flush();
}


void Console::writeLine(const std::string &text) {
    writeLineWithColor(text);
}


void Console::writeDebug(const std::string &text) {
    writeLineWithColor(text, DarkGray);
}


void Console::writeError(const std::string &text) {
    writeLineWithColor(text, Red);
}


void Console::writeErrorInfo(const std::string &text) {
    writeLineWithColor(text, Orange);
}


void Console::writeSuccess(const std::string &text) {
    writeLineWithColor(text, Green);
}


void Console::writeLineWithColor(const std::string &text, Color textColor) {
    beforeWriteLine();
    write(text, textColor);
    write("\n");
    afterWriteLine();
}


void Console::startTask(const std::string &text, int taskNumber, int totalTasks) {
    if (!_currentTask.empty()) {
        finishTask({});
    }
    _currentTaskNumber = taskNumber;
    _currentTotalTasks = totalTasks;
    _currentTask = text;
    writeTaskLine();
}


void Console::finishTask(const std::string &result, Console::Color textColor) {
    if (_currentTask.empty()) {
        return;
    }
    clearTaskLine();
    write(_currentTask);
    write(" ");
    write(result, textColor);
    write("\n");
    _currentTask = {};
}


void Console::writeTaskLine() {
    if (!_useColor) {
        return;
    }
    std::stringstream ss;
    ss << "[" << _currentTaskNumber << "/" << _currentTotalTasks << "] ";
    write(ss.str(), LightBlue);
    _currentTaskLineSize = ss.str().size();
    write(_currentTask, Yellow);
    write(" ...");
    _currentTaskLineSize += _currentTask.size() + 4;
    flush();
}


void Console::clearTaskLine() {
    if (_useColor) {
        for (std::size_t i{}; i < _currentTaskLineSize; ++i) {
            std::cout << '\x08';
        }
        for (std::size_t i{}; i < _currentTaskLineSize; ++i) {
            std::cout << ' ';
        }
        for (std::size_t i{}; i < _currentTaskLineSize; ++i) {
            std::cout << '\x08';
        }
    }
}


void Console::beforeWriteLine() {
    if (!_currentTask.empty()) {
        clearTaskLine();
    }
}


void Console::afterWriteLine() {
    if (!_currentTask.empty()) {
        writeTaskLine();
    }
}


void Console::writeTestEntry(const std::string &type, const MetaData &metaData) {
    write(type);
    write(": ");
    if (metaData.isSkipByDefault()) {
        write("(");
    }
    write(metaData.shortName(), White);
    if (metaData.isSkipByDefault()) {
        write(")");
    }
    if (!metaData.tags().empty()) {
        write(" [", DarkCyan);
        bool first = true;
        for (const auto &tag : metaData.tags()) {
            if (!first) {
                write(", ", DarkCyan);
            }
            write(tag, Cyan);
            first = false;
        }
        write("]", DarkCyan);
    }
    if (!metaData.targets().empty()) {
        write(" <", Violet);
        bool first = true;
        for (const auto &target : metaData.targets()) {
            if (!first) {
                write(", ", Violet);
            }
            write(target, Magenta);
            first = false;
        }
        write(">", Violet);
    }
    write("\n");
}

auto Console::currentTask() const -> std::string {
    return _currentTask;
}


void Console::writeErrorTaskLine(
    const std::string &task,
    const std::string &result,
    Console::Color textColor) {

    write(task);
    write(" ");
    write(result, textColor);
    write("\n");
}


}

