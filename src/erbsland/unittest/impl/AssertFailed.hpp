// Copyright (c) 2023-2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <exception>

namespace erbsland::unittest {

/// @internal
/// The exception which is thrown whenever an assert has failed.
class AssertFailed : public std::exception {
public:
    AssertFailed() = default;
};

}
