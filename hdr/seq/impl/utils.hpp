#pragma once
#include <iterator>
#include <utility>
#include <type_traits>
#include "selectors.hpp"

namespace Utils
{
    namespace _details
    {
        template<typename, typename = void>
        struct is_size_known_at_compiletime : std::false_type {};

        template<typename T>
        struct is_size_known_at_compiletime<T, std::void_t<decltype(std::tuple_size<T>::value)>> : std::true_type {};
    }

    template<typename T>
    using RemoveCVR = std::remove_cv_t<std::remove_reference_t<T>>;

    template<typename Sequence>
    constexpr bool SizeIsKnownAtCompiletime = _details::is_size_known_at_compiletime<RemoveCVR<Sequence>>::value;

    template<typename Sequence>
    constexpr size_t LengthOfSequence = std::tuple_size<RemoveCVR<Sequence>>::value;

    template<typename Sequence>
    using InnerType = RemoveCVR<decltype(*beginSelector(std::declval<Sequence&>()))>;

    template<typename T>
    size_t lengthOfSequence(const T& sequence)
    {
        return std::distance(beginSelector(sequence), endSelector(sequence));
    }

    template<typename Func, typename... Args>
    constexpr bool IsInvocable = std::is_invocable_v<Func, Args...>;

    template<typename Func, typename... Args>
    using ReturnValueOf = std::invoke_result_t<Func, Args...>;
}
