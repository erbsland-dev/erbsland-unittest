// Copyright (c) 2023-2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#include "TestBase.hpp"

#include <algorithm>
#include <iterator>

namespace erbsland::unittest {

TestBase::TestBase(MetaData metaData) :
    _metaData{std::move(metaData)}, _enabled{!_metaData.isSkipByDefault() && !_metaData.isPrintMethod()} {
}

auto TestBase::isEnabled() const -> bool {
    return _enabled;
}

void TestBase::setEnabled(bool enabled) {
    _enabled = enabled;
}

auto TestBase::metaData() const -> const MetaData & {
    return _metaData;
}

auto TestBase::name() const -> const std::string & {
    return _metaData.name();
}

auto TestBase::shortName() const -> const std::string & {
    return _metaData.shortName();
}

}
