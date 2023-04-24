// Copyright © 2023 Tobias Erbsland. Web: https://erbsland.dev
// Copyright © 2023 EducateIT GmbH. Web: https://educateit.ch
//
// This program is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with this program.
// If not, see <https://www.gnu.org/licenses/>.
#pragma once


#include <algorithm>
#include <set>
#include <string>


namespace erbsland::unittest {


enum class FilterOption {
    Exclusive,
    Included,
    Excluded
};


struct FilterRule {
    std::set<std::string> exclusive;
    std::set<std::string> included;
    std::set<std::string> excluded;

    [[nodiscard]] inline auto isEmpty() const -> bool {
        return exclusive.empty() && included.empty() && excluded.empty();
    }

    [[nodiscard]] inline auto toString() const -> std::string {
        std::string result;
        if (!exclusive.empty()) {
            result += "only use";
            std::for_each(exclusive.cbegin(), exclusive.cend(), [&](auto x) { result += " '"; result += x; result += "'"; });
            result += " ";
        }
        if (!included.empty()) {
            result += "include";
            std::for_each(included.cbegin(), included.cend(), [&](auto x) { result += " '"; result += x; result += "'"; });
            result += " ";
        }
        if (!excluded.empty()) {
            result += "exclude";
            std::for_each(excluded.cbegin(), excluded.cend(), [&](auto x) { result += " '"; result += x; result += "'"; });
            result += " ";
        }
        return result;
    }
};


struct FilterSet {
    const std::set<std::string> &names;
    const std::set<std::string> &tags;
    const std::set<std::string> &targets;
};


struct Filter {
    FilterRule names;
    FilterRule tags;
    FilterRule targets;

    [[nodiscard]] inline auto hasExclusiveSet() const -> bool {
        return !names.exclusive.empty() || !tags.exclusive.empty() || !targets.exclusive.empty();
    }

    [[nodiscard]] inline auto set(FilterOption option) const -> FilterSet {
        switch (option) {
            case FilterOption::Exclusive:
            default:
                return FilterSet{names.exclusive, tags.exclusive, targets.exclusive};
            case FilterOption::Included:
                return FilterSet{names.included, tags.included, targets.included};
            case FilterOption::Excluded:
                return FilterSet{names.excluded, tags.excluded, targets.excluded};
        }
    }

    [[nodiscard]] inline auto isEmpty() const -> bool {
        return names.isEmpty() && tags.isEmpty() && targets.isEmpty();
    }

    [[nodiscard]] inline auto toString() const -> std::string {
        std::string result;
        if (!isEmpty()) {
            if (!names.isEmpty()) {
                result += "\n  Names: ";
                result += names.toString();
            }
            if (!tags.isEmpty()) {
                result += "\n  Tags: ";
                result += tags.toString();
            }
            if (!targets.isEmpty()) {
                result += "\n  Targets: ";
                result += names.toString();
            }
        }
        return result;
    }
};


}
