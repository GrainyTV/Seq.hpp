#pragma once
#include "selectors.hpp"
#include <cstdint>

namespace Seq::_internal::TypeInspect
{
    template<typename T>
    using RemoveCVR = std::remove_cvref_t<T>;

    template<typename Func, typename... Args>
    using ReturnValueOf = std::invoke_result_t<Func, Args...>;

    template<typename Func, typename... Args>
    constexpr bool IS_INVOKABLE = std::is_invocable_v<Func, Args...>;

    template<typename SeqT>
    concept EnsureIsSeq = requires (const SeqT& sequence) {
        Seq::_internal::Selectors::beginSelector(sequence);
        Seq::_internal::Selectors::endSelector(sequence);
    };

    template<typename SeqT>
    using ItemOf = RemoveCVR<decltype(*Seq::_internal::Selectors::beginSelector(std::declval<SeqT>()))>;

    template<typename T1, typename T2>
    constexpr bool IS = std::is_same_v<T1, T2>;

    template<typename T>
    constexpr bool LESS_THAN_4_BYTE_INTEGRAL = sizeof(T) < 4;

    template<typename T>
    constexpr bool SIGNED_4_BYTE_INTEGRAL = sizeof(T) == 4 && std::is_signed_v<T>;

    template<typename T>
    constexpr bool IS_SUMMABLE = std::is_integral_v<T> || IS<T, float> || IS<T, double>;

    template<typename T>
    constexpr bool IS_SMALL_INT = LESS_THAN_4_BYTE_INTEGRAL<T> || SIGNED_4_BYTE_INTEGRAL<T>;

    template<typename T>
    using FallbackSumInitial =
        std::conditional_t<IS<T, float>, float,
            std::conditional_t<IS<T, double>, double,
                std::conditional_t<IS_SMALL_INT<T>, int32_t, uint32_t>>>;
}
