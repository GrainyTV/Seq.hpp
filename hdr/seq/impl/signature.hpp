#pragma once
#include <type_traits>

template<bool Invocable, typename Return, typename Func, typename... Args>
struct is_valid : std::false_type {};

template<typename Return, typename Func, typename... Args>
struct is_valid<true, Return, Func, Args...> : std::is_same<std::invoke_result_t<Func, Args...>, Return> {};

template<typename Func, typename... Args>
constexpr bool ValidSignatureNoreturn = is_valid<std::is_invocable_v<Func, Args...>, void, Func, Args...>::value;

template<typename Return, typename Func, typename... Args>
constexpr bool ValidSignature = is_valid<std::is_invocable_v<Func, Args...>, Return, Func, Args...>::value;
