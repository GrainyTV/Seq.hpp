#pragma once
#include "selectors.hpp"

template<typename T, typename = void>
struct is_enumerable : std::false_type {};

template<typename T>
struct is_enumerable<T, std::void_t<
    decltype(beginSelector(std::declval<T>())),
    decltype(endSelector(std::declval<T>()))>> : std::true_type {};

template<typename T>
constexpr bool IEnumerable = is_enumerable<T>::value;
