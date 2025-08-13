#pragma once
#include <iterator>

namespace Utils
{
    template<typename Sequence>
    using InnerType = std::remove_cv_t<std::remove_reference_t<decltype(*beginSelector(std::declval<Sequence&>()))>>;

    template<typename T>
    constexpr size_t lengthOfSequence(const T& sequence)
    {
        return std::distance(beginSelector(sequence), endSelector(sequence));
    }
}
