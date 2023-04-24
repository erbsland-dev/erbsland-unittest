// Copyright © 2023 Tobias Erbsland. Web: https://erbsland.dev
// Copyright © 2023 EducateIT GmbH. Web: https://educateit.ch
//
// This program is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with this program.
// If not, see <https://www.gnu.org/licenses/>.
#pragma once


#include <map>
#include <string>


namespace erbsland::unittest {


class MetaData;


/// @internal
/// Support for enhanced console output.
///
class Console {
public:
    /// The text/background color
    enum Color : int {
        Default = -1,
        Black = 0,
        DarkRed = 1,
        Green = 2,
        Orange = 3,
        DarkBlue = 4,
        Violet = 5,
        DarkCyan = 6,
        White = 7,
        DarkGray = 8,
        Red = 9,
        LightGreen = 10,
        Yellow = 11,
        LightBlue = 12,
        Magenta = 13,
        Cyan = 6,
        BrightWhite = 7,
    };

public:
    /// ctor
    ///
    Console();

public: // settings
    /// Set if colour shall be used.
    ///
    void setUseColor(bool enabled);

public: // usage
    /// Write a regular line of text.
    ///
    void writeLine(const std::string &text);

    /// Write debug messages (adds a newline).
    ///
    void writeDebug(const std::string &text);

    /// Write an error message (adds a newline).
    ///
    void writeError(const std::string &text);

    /// Write an error location message (adds a newline).
    ///
    void writeErrorInfo(const std::string &text);

    /// Write a success message.
    ///
    void writeSuccess(const std::string &text);

    /// Write a test entry.
    ///
    void writeTestEntry(const std::string &type, const MetaData &metaData);

public: // status handling.
    /// Start a new task.
    ///
    /// Starts a new task, writing a line "[n/m] <task text> ...".
    /// Any call to `writeXXX()` will write this text "above" the status line keep the status visible.
    ///
    /// @param taskNumber The task number.
    /// @param totalTasks The total number of tasks.
    ///
    void startTask(const std::string &text, int taskNumber, int totalTasks);

    /// Finish a task.
    ///
    /// Finishes the task, by replacing the status line with "<task text> <result>".
    ///
    void finishTask(const std::string &result, Color textColor = Default);

    /// Get the current task.
    ///
    [[nodiscard]] auto currentTask() const -> std::string;

    /// Write a task line for error reporting.
    ///
    void writeErrorTaskLine(
        const std::string &task,
        const std::string &result,
        Color textColor);

private: // low level API
    /// Write a line in a given color.
    ///
    void writeLineWithColor(const std::string &text, Color textColor = Default);

    /// Write text.
    ///
    /// @param text The text to write.
    /// @param textColor The colour of the text.
    ///
    void write(const std::string &text, Color textColor = Default);

    /// Flush the output buffer.
    ///
    void flush();

    /// Called before a write line.
    ///
    void beforeWriteLine();

    /// Called after a write line.
    ///
    void afterWriteLine();

    /// Write the current task line.
    ///
    void writeTaskLine();

    /// Clear the task line.
    ///
    void clearTaskLine();

private:
    bool _useColor{true};
    int _currentTaskNumber{0};
    int _currentTotalTasks{0};
    std::string _currentTask{};
    std::size_t _currentTaskLineSize{0};
    std::map<Color, std::tuple<int, int>> _colorMap;
};


}
