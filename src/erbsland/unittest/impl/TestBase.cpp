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
#include "TestBase.hpp"


#include <algorithm>
#include <iterator>


namespace erbsland::unittest {


TestBase::TestBase(MetaData metaData)
:
    _metaData{std::move(metaData)},
    _enabled{!_metaData.isSkipByDefault()&&!_metaData.isPrintMethod()} {
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