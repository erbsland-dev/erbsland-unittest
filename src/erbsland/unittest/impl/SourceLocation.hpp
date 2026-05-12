// Copyright (c) 2023-2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#pragma once

namespace erbsland::unittest {

/// @internal
/// The source location in the unittest code
struct SourceLocation {
    const char *file;
    int lineNo;
};

}
