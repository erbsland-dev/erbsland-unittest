// Copyright © 2025 EducateIT GmbH (https://educateit.ch/) and Tobias Erbsland (https://erbsland.dev/)
// All rights are reserved. Detailed terms are specified in the "COPYRIGHT.md" file.
#include "ConsoleColor.hpp"


#include <sstream>


namespace erbsland::unittest {


auto ConsoleColor::foreground() const noexcept -> std::string {
    std::stringstream result;
    result << "\x1b[" << std::get<0>(colorMap().at(_value)) << "m";
    return result.str();
}


auto ConsoleColor::background() const noexcept -> std::string {
    std::stringstream result;
    result << "\x1b[" << std::get<1>(colorMap().at(_value)) << "m";
    return result.str();
}


auto ConsoleColor::colorMap() noexcept -> const std::map<Value, std::tuple<int, int>> & {
    const static std::map<Value, std::tuple<int, int>> map {
        {Black,       std::make_tuple(30, 40)},
        {DarkRed,     std::make_tuple(31, 41)},
        {Green,       std::make_tuple(32, 42)},
        {Orange,      std::make_tuple(33, 43)},
        {DarkBlue,    std::make_tuple(34, 44)},
        {Violet,      std::make_tuple(35, 45)},
        {DarkCyan,    std::make_tuple(36, 46)},
        {White,       std::make_tuple(37, 47)},
        {Default,     std::make_tuple(39, 49)},
        {DarkGray,    std::make_tuple(90, 90)},
        {Red,         std::make_tuple(91, 101)},
        {LightGreen,  std::make_tuple(92, 102)},
        {Yellow,      std::make_tuple(93, 103)},
        {LightBlue,   std::make_tuple(94, 104)},
        {Magenta,     std::make_tuple(95, 105)},
        {Cyan,        std::make_tuple(96, 106)},
        {BrightWhite, std::make_tuple(97, 107)},
    };
    return map;
}


}

