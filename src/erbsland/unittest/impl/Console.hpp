// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "ConsoleLine.hpp"

#include <map>
#include <mutex>
#include <string>
#include <thread>


namespace erbsland::unittest {


class MetaData;


/// @internal
/// Support for enhanced console output.
///
class Console {
public:
    struct TaskInfo {
        std::string text;
        int taskNumber{};
        int totalTasks{};
    };

public:
    /// ctor
    ///
    Console() = default;

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

    /// Reset the formatting at the end of the output.
    ///
    void resetFormatting();

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
    void finishTask(const std::string &result, ConsoleColor textColor = {});

    /// Write a task line for error reporting.
    ///
    void writeErrorTaskLine(const std::string &task, const std::string &result, ConsoleColor textColor);

private: // low level API
    /// Write a line in a given color.
    ///
    void writeLineWithColor(const std::string &text, ConsoleColor textColor = {});

    /// Prepare a task line.
    ///
    /// @param taskInfo The information about the task to prepare.
    /// @param status An optional status. Empty means "running".
    /// @param statusColor An optional status color, only used if a status is set.
    /// @return The prepared status line.
    ///
    [[nodiscard]] auto createTaskLine(
        const TaskInfo &taskInfo,
        const std::string &status,
        ConsoleColor statusColor) noexcept -> ConsoleLine;

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

    /// Send a line to the console.
    ///
    void sendLineSynchronized(const ConsoleLine &line) noexcept;

private:
    bool _useColor{true}; ///< Flag if coloured output shall be used.
    mutable std::mutex _mutex; ///< A mutex to synchronize the output lines.
    TaskInfo _currentTask; ///< Information aber the currently running task.
    ConsoleLine _currentTaskLine; ///< The current formatted task line.
};


}
