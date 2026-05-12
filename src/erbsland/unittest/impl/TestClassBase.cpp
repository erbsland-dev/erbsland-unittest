// Copyright (c) 2023-2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#include "TestClassBase.hpp"

namespace erbsland::unittest {

TestClassBase::TestClassBase(MetaData metaData) : _metaData{std::move(metaData)} {
}

auto TestClassBase::metaData() const -> const MetaData & {
    return _metaData;
}

auto TestClassBase::name() const -> const std::string & {
    return _metaData.name();
}

auto TestClassBase::shortName() const -> const std::string & {
    return _metaData.shortName();
}

}
