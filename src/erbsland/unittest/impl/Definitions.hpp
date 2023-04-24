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


// This is a copy of some of the definitions found in the `Definitions.hpp` file from ErbslandCore.
// It is included in ErbslandUnitTest to keep this project dependency free.


#include <cassert>
#include <cstdint>
#include <cstdlib>


// Roughly detect the platform and compiler for API access.
#if _WIN32 || _WIN64
#define ERBSLAND_OS_WINDOWS
#ifndef EERBSLAND_NO_SHORT_NAMESPACE
#define EL_OS_WINDOWS
#endif
#elif __APPLE__
#define ERBSLAND_OS_MACOS
#ifndef EERBSLAND_NO_SHORT_NAMESPACE
#define EL_OS_MACOS
#endif
#else
#define ERBSLAND_OS_UNIX
#ifndef EERBSLAND_NO_SHORT_NAMESPACE
#define EL_OS_UNIX
#endif
#endif


// Roughly detect the compiler.
#if defined(__clang__)
#define ERBSLAND_COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
#define ERBSLAND_COMPILER_GCC
#elif defined(_MSC_VER)
#define ERBSLAND_COMPILER_MSVC
#endif


// Basic checks about the environment.
#ifdef ERBSLAND_OS_WINDOWS
#if _WIN64
#define ERBSLAND_ARCH_64
#else
#define ERBSLAND_ARCH_32
#endif
#else
#if __x86_64__ || __ppc64__
#define ERBSLAND_ARCH_64
#else
#define ERBSLAND_ARCH_32
#endif
#endif


// Check the size of wchar_t
#ifdef ERBSLAND_OS_WINDOWS
#define ERBSLAND_WCHAR_16BIT
static_assert(sizeof(wchar_t) == 2);
#else
#define ERBSLAND_WCHAR_32BIT
static_assert(sizeof(wchar_t) == 4);
#endif


// Define the `erbsland::unittest` namespace and import it into the `erbsland` namespace.
namespace erbsland {
namespace unittest {}
using namespace ::erbsland::unittest;
};


// Allow to disable the short namespace if it causes conflicts.
#ifndef ERBSLAND_NO_SHORT_NAMESPACE
#ifdef ERBSLAND_SHORT_NAMESPACE
namespace ERBSLAND_SHORT_NAMESPACE = erbsland;
#else
namespace el = erbsland;
#endif
#endif


