// Copyright (c) 2023-2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#include "Demangle.hpp"

#include <memory>

#ifndef ERBSLAND_OS_WINDOWS
#include <cxxabi.h>
#endif

namespace erbsland::unittest {

auto demangleTypeName(const std::string &name) -> std::string {
#ifdef ERBSLAND_OS_WINDOWS
    return name;
#else
    int status;
    char *demangledName = abi::__cxa_demangle(name.c_str(), nullptr, 0, &status);
    if (status == 0 && demangledName != nullptr) {
        auto result = std::string(demangledName);
        free(demangledName);
        return result;
    } else {
        free(demangledName);
        return name;
    }
#endif
}

}
