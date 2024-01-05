// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


namespace erbsland::unittest {


/// @internal
/// Flags to control assert evaluation.
///
enum AssertFlags : int {
    AssertCheck = (1<<0), ///< Only check the assertion, do not stop the test if it fails.
    AssertNegate = (1<<1), ///< Check if the expression is false.
};


}

