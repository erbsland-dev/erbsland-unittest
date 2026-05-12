// Copyright (c) 2023-2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#pragma once

namespace erbsland::unittest {

/// @internal
/// Flags to control assert evaluation.
enum AssertFlags : int {
    AssertCheck = (1 << 0),  ///< Only check the assertion, do not stop the test if it fails.
    AssertNegate = (1 << 1), ///< Check if the expression is false.
};

}
