// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


namespace erbsland::unittest {


/// @internal
/// The source location in the unittest code
///
struct SourceLocation {
    const char *file;
    int lineNo;
};


}

