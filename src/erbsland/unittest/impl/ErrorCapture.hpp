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

