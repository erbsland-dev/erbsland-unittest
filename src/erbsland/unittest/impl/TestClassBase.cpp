// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "TestClassBase.hpp"


namespace erbsland::unittest {


TestClassBase::TestClassBase(MetaData metaData)
    : _metaData{std::move(metaData)} {
}


auto TestClassBase::metaData() const -> const MetaData& {
    return _metaData;
}


auto TestClassBase::name() const -> const std::string& {
    return _metaData.name();
}


auto TestClassBase::shortName() const -> const std::string& {
    return _metaData.shortName();
}


}


