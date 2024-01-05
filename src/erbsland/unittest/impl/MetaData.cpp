// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "MetaData.hpp"


namespace erbsland::unittest {


MetaData::MetaData(const std::string& name)
    : _name{name}, _shortName{name}, _flags{0} {
}


MetaData::MetaData(
    const std::string &name,
    const std::vector<std::string> &tags,
    const std::vector<std::string> &testedTargets,
    Flags flags) noexcept
:
    _name{name},
    _shortName{name},
    _flags{flags} {

    for (const auto &tag : tags) {
        _tags.insert(tag);
    }
    for (const auto &testedTarget : testedTargets) {
        _targets.insert(testedTarget);
    }

    if (name.size() > 4 && name.substr(name.size() - 4) == "Test") {
        // for classes, remove the "...Test" suffix.
        _shortName = name.substr(0, name.size() - 4);
    }
    if (auto seperatorIndex = name.find("::"); seperatorIndex != std::string::npos) {
        _shortName = name.substr(seperatorIndex + 2);
    }
    if (_shortName.size() > 4 && _shortName.substr(0, 4) == "test") {
        // for tests, remove the "test..." prefix.
        _shortName = name.substr(4);
    } else if (_shortName.size() > 5 && _shortName.substr(0, 5) == "print") {
        _flags |= PrintMethod;
        _shortName = name.substr(5);
    }
}


auto MetaData::matches(const Filter &filter, FilterOption option) const noexcept -> bool {
    const auto &filterSet = filter.set(option);
    if (filterSet.names.find(_name) != filterSet.names.cend()) {
        return true;
    }
    if (filterSet.names.find(_shortName) != filterSet.names.cend()) {
        return true;
    }
    if (std::any_of(_tags.cbegin(), _tags.cend(), [&](const auto &tag) -> bool {
        return filterSet.tags.find(tag) != filterSet.tags.cend(); })) {
        return true;
    }
    if (std::any_of(_targets.cbegin(), _targets.cend(), [&](const auto &target) -> bool {
        return filterSet.targets.find(target) != filterSet.targets.cend(); })) {
        return true;
    }
    return false;
}


auto MetaData::name() const noexcept -> const std::string & {
    return _name;
}


auto MetaData::shortName() const noexcept -> const std::string & {
    return _shortName;
}


auto MetaData::tags() const noexcept -> const std::set<std::string> & {
    return _tags;
}


auto MetaData::targets() const noexcept -> const std::set<std::string> & {
    return _targets;
}


auto MetaData::isSkipByDefault() const noexcept -> bool {
    return (_flags & SkipByDefault) != 0;
}

auto MetaData::isPrintMethod() const noexcept -> bool {
    return (_flags & PrintMethod) != 0;
}


}
