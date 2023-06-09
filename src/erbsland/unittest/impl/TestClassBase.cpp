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


