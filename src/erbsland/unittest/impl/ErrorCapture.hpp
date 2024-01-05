// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "Console.hpp"

#include <list>
#include <memory>


namespace erbsland::unittest {


/// A captured error, to be displayed at the end of the tests.
///
class ErrorCapture {
public:
    /// Create a new error capture.
    ///
    ErrorCapture(
        const std::string &suite,
        const std::string &test,
        const std::string &result,
        Console::Color resultColor);

public:
    /// Add context information.
    ///
    void addContextInfo(const std::string &infoLine);

    /// Add debug information.
    ///
    void addDebugInfo(const std::string &debugLine);

public:
    [[nodiscard]] auto suite() const -> const std::string&;
    [[nodiscard]] auto test() const -> const std::string&;
    [[nodiscard]] auto result() const -> const std::string&;
    [[nodiscard]] auto resultColor() const -> Console::Color;
    [[nodiscard]] auto contextInfo() const -> const std::list<std::string>&;
    [[nodiscard]] auto debugInfo() const -> const std::list<std::string>&;

private:
    std::string _suite;
    std::string _test;
    std::string _result;
    Console::Color _resultColor;
    std::list<std::string> _contextInfo;
    std::list<std::string> _debugInfo;
};


using ErrorCapturePtr = std::shared_ptr<ErrorCapture>;


}

