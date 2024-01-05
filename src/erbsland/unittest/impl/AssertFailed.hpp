// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include <exception>


namespace erbsland::unittest {


/// @internal
/// The exception which is thrown whenever an assert has failed.
///
class AssertFailed : public std::exception
{
public:
    AssertFailed() = default;
};


}

