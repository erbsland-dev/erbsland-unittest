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
#include "ErrorCapture.hpp"


namespace erbsland::unittest {


ErrorCapture::ErrorCapture(
    const std::string &suite,
    const std::string &test,
    const std::string &result,
    Console::Color resultColor)
:
    _suite{suite},
    _test{test},
    _result{result},
    _resultColor{resultColor} {
}


void ErrorCapture::addContextInfo(const std::string &infoLine) {
    _contextInfo.push_back(infoLine);
}


void ErrorCapture::addDebugInfo(const std::string &debugLine) {
    _debugInfo.push_back(debugLine);
}


auto ErrorCapture::suite() const -> const std::string & {
    return _suite;
}


auto ErrorCapture::test() const -> const std::string & {
    return _test;
}


auto ErrorCapture::result() const -> const std::string & {
    return _result;
}


auto ErrorCapture::resultColor() const -> Console::Color {
    return _resultColor;
}


auto ErrorCapture::contextInfo() const -> const std::list<std::string> & {
    return _contextInfo;
}


auto ErrorCapture::debugInfo() const -> const std::list<std::string> & {
    return _debugInfo;
}


}

