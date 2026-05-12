// Copyright (c) 2023-2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#pragma once

namespace erbsland::unittest {

/// @internal
/// The result of an assert evaluation.
enum AssertResult {
    ExpectedResult,      ///< Expected expression result.
    UnexpectedResult,    ///< Not expected expression result.
    UnexpectedException, ///< Exception when no was expected.
};

}
