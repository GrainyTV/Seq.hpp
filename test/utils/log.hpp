#pragma once
#include "formatters.hpp"

#include <iostream>
#include <string_view>
#include <utility>

namespace AnsiColor
{
    constexpr int RED   = 0x1f;
    constexpr int GREEN = 0x20;
}

namespace Log
{
    constexpr const char* ANSI_BEGIN = "\033[";
    constexpr const char* ANSI_RESET = "\033[0m";

    inline auto printBdColor(std::string_view text, int color) -> void
    {
        std::cout << ANSI_BEGIN << "1;" << color << "m" << text << ANSI_RESET;
    }

    template<typename... Args>
    inline auto println(std::format_string<Args...> fmt, Args&&... args) -> void
    {
        std::cout << std::format(fmt, std::forward<Args>(args)...) << '\n';
    }
}
