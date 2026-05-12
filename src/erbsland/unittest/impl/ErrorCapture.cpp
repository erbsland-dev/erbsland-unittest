// Copyright (c) 2023-2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#include "ErrorCapture.hpp"

#include <utility>

namespace erbsland::unittest {

ErrorCapture::ErrorCapture(std::string suite, std::string test, std::string result, const ConsoleColor resultColor) :
    _suite{std::move(suite)}, _test{std::move(test)}, _result{std::move(result)}, _resultColor{resultColor} {
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

auto ErrorCapture::resultColor() const -> ConsoleColor {
    return _resultColor;
}

auto ErrorCapture::contextInfo() const -> const std::list<std::string> & {
    return _contextInfo;
}

auto ErrorCapture::debugInfo() const -> const std::list<std::string> & {
    return _debugInfo;
}

}
