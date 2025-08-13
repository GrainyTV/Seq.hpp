#pragma once
#include <utility>
#include <filesystem>
#include <cstring>

namespace fs = std::filesystem;

// ┏━━━━━━━━━━━━━━━━━┓
// ┃ C-style strings ┃
// ┗━━━━━━━━━━━━━━━━━┛

// Small hack is needed here to prevent the compiler
// from decaying character arrays to pointers therefore
// causing to return the wrong size (\0 should not be included)

template<typename T, std::enable_if_t<std::is_pointer_v<T> && std::is_same_v<T, const char*>, bool> = true>
constexpr auto beginSelector(T str)
{
    return str;
}

template<typename T, std::enable_if_t<std::is_pointer_v<T> && std::is_same_v<T, const char*>, bool> = true>
constexpr auto endSelector(T str)
{
    return str + std::strlen(str);
}

// ┏━━━━━━━━━━━━━━━━┓
// ┃ C-style arrays ┃
// ┗━━━━━━━━━━━━━━━━┛

template<typename T, std::size_t N>
constexpr auto beginSelector(T(&array)[N])
{
    return array;
}

template<typename T, std::size_t N>
constexpr auto endSelector(T(&array)[N])
{
    return array + N;
}

// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃ Types with builtin begin and end member functions ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

template<typename Seq>
using BuiltinBegin = decltype(std::declval<Seq&>().begin());

template<typename Seq>
using BuiltinEnd = decltype(std::declval<Seq&>().end());

template<typename Seq>
using BuiltinBeginConst = decltype(std::declval<const Seq&>().begin());

template<typename Seq>
using BuiltinEndConst = decltype(std::declval<const Seq&>().end());

template<typename Seq>
constexpr auto beginSelector(Seq& sequence) -> BuiltinBegin<Seq>
{
    return sequence.begin();
}

template<typename Seq>
constexpr auto endSelector(Seq& sequence) -> BuiltinEnd<Seq>
{
    return sequence.end();
}

template<typename Seq>
constexpr auto beginSelector(const Seq& sequence) -> BuiltinBeginConst<Seq>
{
    return sequence.begin();
}

template<typename Seq>
constexpr auto endSelector(const Seq& sequence) -> BuiltinEndConst<Seq>
{
    return sequence.end();
}

// ┏━━━━━━━━━━━━━━━━━━━━┓
// ┃ Directory iterator ┃
// ┗━━━━━━━━━━━━━━━━━━━━┛

inline auto beginSelector(const fs::directory_iterator& dir) -> fs::directory_iterator
{
    return dir;
}

inline auto endSelector(const fs::directory_iterator&) -> fs::directory_iterator
{
    return {};
}
