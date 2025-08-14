#pragma once
#include "utils.hpp"
#include <type_traits>

template<bool Invocable, typename Return, typename Func, typename... Args>
struct is_signature_valid : std::false_type {};

template<typename Return, typename Func, typename... Args>
struct is_signature_valid<true, Return, Func, Args...> : std::is_same<Utils::ReturnValueOf<Func, Args...>, Return> {};

template<typename Func, typename... Args>
constexpr bool ValidSignatureNoreturn = is_signature_valid<Utils::IsInvocable<Func, Args...>, void, Func, Args...>::value;

template<typename Return, typename Func, typename... Args>
constexpr bool ValidSignature =
    std::is_same_v<Return, void> == false && // should use noreturn version with void functions
    is_signature_valid<Utils::IsInvocable<Func, Args...>, Return, Func, Args...>::value;
