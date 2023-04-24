# Copyright © 2023 Tobias Erbsland. Web: https://erbsland.dev
# Copyright © 2023 EducateIT GmbH. Web: https://educateit.ch
#
# This program is free software: you can redistribute it and/or modify it under the terms of the
# GNU Lesser General Public License as published by the Free Software Foundation, either
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License along with this program.
# If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.20)

add_library(erbsland-unittest STATIC)
set_target_properties(erbsland-unittest PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(erbsland-unittest PRIVATE cxx_std_17)
target_include_directories(erbsland-unittest INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")

add_subdirectory(src/erbsland/unittest)

include(tools/unittest/unittest.cmake)

