// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


namespace erbsland::unittest {


/// @internal
/// The result of an assert evaluation.
///
enum AssertResult {
    ExpectedResult, ///< Expected expression result.
    UnexpectedResult, ///< Not expected expression result.
    UnexpectedException, ///< Exception when no was expected.
};


}

