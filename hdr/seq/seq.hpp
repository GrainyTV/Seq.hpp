#pragma once
#include "impl/assert.hpp"
#include "impl/ienumerable.hpp"
#include "impl/signature.hpp"
#include "impl/utils.hpp"
#include "impl/extras.hpp"
#include <type_traits>
#include <algorithm>
#include <cstddef>
#include <numeric>
#include <vector>

template<typename T, typename Closure, std::enable_if_t<IEnumerable<T>, bool> = true>
constexpr auto operator|(T&& sequence, Closure&& callWithSequence)
{
    return callWithSequence(sequence);
}

namespace Seq
{
    template<typename Action>
    constexpr auto iter(Action&& action)
    {
        return [action](const auto& sequence) -> void
        {
            using T = Utils::InnerType<decltype(sequence)>;
            static_assert(ValidSignatureNoreturn<Action, T>, "Signature of Action must be <(T) -> void>");

            std::for_each(beginSelector(sequence), endSelector(sequence), action);
        };
    }

    template<typename Action>
    constexpr auto iteri(Action&& action)
    {
        return [action](const auto& sequence) -> void
        {
            using T = Utils::InnerType<decltype(sequence)>;
            static_assert(ValidSignatureNoreturn<Action, std::size_t, T>, "Signature of Action must be <(size_t, T) -> void>");

            std::size_t index = 0;
            std::for_each(beginSelector(sequence), endSelector(sequence), [&index, action](const T& elem)
            {
                action(index, elem);
                ++index;
            });
        };
    }

    template<int32_t InclusiveStart, int32_t ExclusiveStop>
    constexpr auto range()
    {
        static_assert(ExclusiveStop - InclusiveStart > 0, "ExclusiveStop must be larger than InclusiveStart");
        constexpr size_t N = ExclusiveStop - InclusiveStart;

        std::array<int32_t, N> items;
        std::iota(beginSelector(items), endSelector(items), InclusiveStart);

        return items;
    }

    template<int32_t ExclusiveStop>
    constexpr auto range()
    {
        return range<0, ExclusiveStop>();
    }

    inline auto range(int32_t inclusiveStart, int32_t exclusiveStop)
    {
        ASSERT(exclusiveStop - inclusiveStart > 0);
        const size_t N = exclusiveStop - inclusiveStart;

        std::vector<int32_t> items(N);
        std::iota(beginSelector(items), endSelector(items), inclusiveStart);

        return items;
    }

    inline auto range(int32_t exclusiveStop)
    {
        return range(0, exclusiveStop);
    }

    template<typename Predicate>
    auto filter(Predicate&& predicate)
    {
        return [predicate](const auto& sequence)
        {
            using T = Utils::InnerType<decltype(sequence)>;
            static_assert(ValidSignature<bool, Predicate, T>, "Signature of Predicate must be <(T) -> bool>");

            size_t N = Utils::lengthOfSequence(sequence);

            std::vector<T> items;
            items.reserve(N);
            std::copy_if(beginSelector(sequence), endSelector(sequence), std::back_inserter(items), predicate);

            return items;
        };
    }
}
