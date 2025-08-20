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
    namespace _details
    {
        template<typename Action, typename Seq>
        constexpr auto iterMutual(Action&& action, Seq&& sequence)
        {
            std::for_each(beginSelector(sequence), endSelector(sequence), action);
        }

        template<typename U, typename Seq, typename T, typename Mapping>
        constexpr auto mapMutual(Mapping&& mapping, Seq&& sequence)
        {
            if constexpr (Utils::SizeIsKnownAtCompiletime<Seq>)
            {
                constexpr size_t N = Utils::LengthOfSequence<Seq>;

                std::array<U, N> items;
                std::transform(beginSelector(sequence), endSelector(sequence), beginSelector(items), mapping);

                return items;
            }
            else
            {
                const size_t N = Utils::lengthOfSequence(sequence);

                std::vector<U> items(N);
                std::transform(beginSelector(sequence), endSelector(sequence), beginSelector(items), mapping);

                return items;
            }
        }

        template<typename T>
        constexpr auto sumStartingValue()
        {
            if constexpr (std::is_integral_v<T> && sizeof(T) < 4)
            {
                return 0; // prevent using smaller than basic 4 byte integers for sum
            }
            else
            {
                return static_cast<T>(0);
            }
        }
    }

    template<typename Action>
    constexpr auto iter(Action&& action)
    {
        return [action](const auto& sequence) -> void
        {
            using T = Utils::InnerType<decltype(sequence)>;
            static_assert(ValidSignatureNoreturn<Action, T>, "Signature of Action must be <(T) -> void>");

            _details::iterMutual(action, sequence);
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
            auto indexedAction = [&index, action](const T& elem)
            {
                action(index, elem);
                ++index;
            };

            _details::iterMutual(indexedAction, sequence);
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

    constexpr inline auto pairwise()
    {
        return [](const auto& sequence)
        {
            using T = Utils::InnerType<decltype(sequence)>;
            size_t N = Utils::lengthOfSequence(sequence) - 1;

            std::vector<std::pair<T, T>> items(N);
            std::transform(beginSelector(sequence),
                           endSelector(sequence) - 1, // we are consuming two elements at once, stop one iteration earlier
                           beginSelector(sequence) + 1,
                           beginSelector(items), [](const T& former, const T& latter) { return std::make_pair(former, latter); });

            return items;
        };
    }

    constexpr auto pairwiseWrap()
    {
        return [](const auto& sequence)
        {
            using T = Utils::InnerType<decltype(sequence)>;
            size_t N = Utils::lengthOfSequence(sequence);

            auto basePairs = sequence | Seq::pairwise();

            std::vector<std::pair<T, T>> items(N);
            items.back() = std::make_pair(std::get<1>(basePairs.back()), std::get<0>(basePairs.front()));
            std::move(beginSelector(basePairs), endSelector(basePairs), beginSelector(items));

            return items;
        };
    }

    template<typename Predicate>
    constexpr auto forall(Predicate&& predicate)
    {
        return [predicate](const auto& sequence) -> bool
        {
            using T = Utils::InnerType<decltype(sequence)>;
            static_assert(ValidSignature<bool, Predicate, T>, "Signature of Predicate must be <(T) -> bool>");

            return std::all_of(beginSelector(sequence), endSelector(sequence), predicate);
        };
    }

    template<typename Mapping>
    constexpr auto map(Mapping&& mapping)
    {
        return [mapping](const auto& sequence)
        {
            using Seq = decltype(sequence);
            using T = Utils::InnerType<Seq>;
            static_assert(Utils::IsInvocable<Mapping, T>, "Function parameter differs from inner type of enumerable");

            using U = Utils::ReturnValueOf<Mapping, T>;
            static_assert(ValidSignature<U, Mapping, T>, "Signature of Mapping must be <(T) -> U>");

            return _details::mapMutual<U, Seq, T>(mapping, sequence);
        };
    }

    template<typename Mapping>
    constexpr auto mapi(Mapping&& mapping)
    {
        return [mapping](const auto& sequence)
        {
            using Seq = decltype(sequence);
            using T = Utils::InnerType<Seq>;
            static_assert(Utils::IsInvocable<Mapping, size_t, T>, "Function parameters differ from inner type of enumerable");

            using U = Utils::ReturnValueOf<Mapping, size_t, T>;
            static_assert(ValidSignature<U, Mapping, size_t, T>, "Signature of Mapping must be <(size_t, T) -> U>");

            size_t index = 0;
            auto indexedMapping = [&index, mapping](const T& elem)
            {
                const auto result = mapping(index, elem);
                ++index;
                return result;
            };

            return _details::mapMutual<U, Seq, T>(indexedMapping, sequence);
        };
    }

    template<size_t Count, typename Mapping>
    constexpr auto init(Mapping&& mapping)
    {
        static_assert(Utils::IsInvocable<Mapping, decltype(Count)>, "Function parameter is expected to be size_t");

        return Seq::range<Count>() | Seq::map(mapping);
    }

    template<typename Mapping>
    constexpr auto init(size_t count, Mapping&& mapping)
    {
        static_assert(Utils::IsInvocable<Mapping, decltype(count)>, "Function parameter is expected to be size_t");

        return Seq::range(count) | Seq::map(mapping);
    }

    template<typename NewSeq>
    constexpr auto append(NewSeq&& newSequence)
    {
        return [newSequence](const auto& sequence)
        {
            using Seq = decltype(sequence);
            using T1 = Utils::InnerType<Seq>;
            using T2 = Utils::InnerType<NewSeq>;
            static_assert(std::is_same_v<T1, T2>, "Cannot append enumerables of different types");

            if constexpr (Utils::SizeIsKnownAtCompiletime<Seq> && Utils::SizeIsKnownAtCompiletime<NewSeq>)
            {
                constexpr size_t N1 = Utils::LengthOfSequence<Seq>;
                constexpr size_t N = N1 + Utils::LengthOfSequence<NewSeq>;

                std::array<T1, N> items;
                std::copy(beginSelector(sequence), endSelector(sequence), beginSelector(items));
                std::copy(beginSelector(newSequence), endSelector(newSequence), beginSelector(items) + N1);

                return items;
            }
            else
            {
                size_t N1 = Utils::lengthOfSequence(sequence);
                size_t N = N1 + Utils::lengthOfSequence(newSequence);

                std::vector<T1> items(N);
                std::copy(beginSelector(sequence), endSelector(sequence), beginSelector(items));
                std::copy(beginSelector(newSequence), endSelector(newSequence), beginSelector(items) + N1);

                return items;
            }
        };
    }

    constexpr auto sum()
    {
        return [](const auto& sequence)
        {
            using T = Utils::InnerType<decltype(sequence)>;
            static_assert(std::is_arithmetic_v<T>, "Sum expects an an arithmetic enumerable");

            return std::reduce(beginSelector(sequence), endSelector(sequence), _details::sumStartingValue<T>());
        };
    }

    template<typename T>
    constexpr auto singleton(T&& elem)
    {
        return std::array<T, 1>({ elem });
    }

    template<typename T>
    constexpr auto empty()
    {
        return std::array<T, 0>();
    }

    constexpr auto length()
    {
        return [](const auto& sequence) -> size_t
        {
            using Seq = decltype(sequence);

            if constexpr (Utils::SizeIsKnownAtCompiletime<Seq>)
            {
                return Utils::LengthOfSequence<Seq>;
            }
            else
            {
                return Utils::lengthOfSequence(sequence);
            }
        };
    }

    constexpr auto isEmpty()
    {
        return [](const auto& sequence) -> bool
        {
            return (sequence | Seq::length()) == 0;
        };
    }

    template<size_t Count>
    constexpr auto skip()
    {
        return [](const auto& sequence)
        {
            using Seq = decltype(sequence);
            using T = Utils::InnerType<Seq>;

            if constexpr (Utils::SizeIsKnownAtCompiletime<Seq>)
            {
                constexpr size_t N = Utils::LengthOfSequence<Seq>;
                static_assert(Count < N, "Count must not exceed the number of elements in the enumarable");

                std::array<T, N - Count> items;
                std::copy(beginSelector(sequence) + Count, endSelector(sequence), beginSelector(items));

                return items;
            }
            else
            {
                size_t N = Utils::lengthOfSequence(sequence);
                // TODO: runtime assertion implementation

                std::vector<T> items(N - Count);
                std::copy(beginSelector(sequence) + Count, endSelector(sequence), beginSelector(items));

                return items;
            }
        };
    }

    constexpr auto tail()
    {
        return [](const auto& sequence)
        {
            return sequence | Seq::skip<1>();
        };
    }

    template<size_t Count>
    constexpr auto take()
    {
        return [](const auto& sequence)
        {
            using Seq = decltype(sequence);
            using T = Utils::InnerType<Seq>;

            if constexpr (Utils::SizeIsKnownAtCompiletime<Seq>)
            {
                constexpr size_t N = Utils::LengthOfSequence<Seq>;
                static_assert(Count <= N, "Count must not exceed the number of elements in the enumarable");

                std::array<T, Count> items;
                std::copy(beginSelector(sequence), endSelector(sequence) - (N - Count), beginSelector(items));

                return items;
            }
            else
            {
                size_t N = Utils::lengthOfSequence(sequence);
                // TODO: runtime assertion implementation

                std::vector<T> items(Count);
                std::copy(beginSelector(sequence), endSelector(sequence) - (N - Count), beginSelector(items));

                return items;
            }
        };
    }
}
