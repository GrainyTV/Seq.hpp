#pragma once
#include "selectors.hpp"

template<typename T>
using RemoveCVR = std::remove_cvref_t<T>;

template<typename Func, typename... Args>
using ReturnValueOf = std::invoke_result_t<Func, Args...>;

template<typename Func, typename... Args>
constexpr bool IS_INVOKABLE = std::is_invocable_v<Func, Args...>;

template<typename SeqT>
concept EnsureIsSeq = requires(const SeqT& sequence) {
    Seq::_internal::Selectors::beginSelector(sequence);
    Seq::_internal::Selectors::endSelector(sequence);
};

template<typename SeqT>
using ItemOf = RemoveCVR<decltype(*Seq::_internal::Selectors::beginSelector(std::declval<SeqT>()))>;
