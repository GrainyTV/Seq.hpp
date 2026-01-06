#pragma once
#include "lib/config.hpp"
#include "lib/seq_nocapture.hpp"
#include "lib/type_inspect_utils.hpp"

#include <optional>
#include <string>

template<typename Seq>
IEnumerable<ItemOf<Seq>> wrapAsIEnumerable(ByValue<Seq> sequence)
{
    for (const auto& elem : static_cast<Seq>(sequence))
    {
        co_yield elem;
    }
}

template<typename Func, typename T>
auto operator|(IEnumerable<T>&& enumerable, Func&& function)
{
    static_assert(IS_INVOKABLE<Func, IEnumerable<T>>,
                  "Right hand side of pipe operator is NOT a proper IEnumerable closure in this context");
    return std::forward<Func>(function)(std::move(enumerable));
}

template<typename Func, typename T>
auto operator|(IEnumerable<T>& enumerable, Func&& function)
{
    return std::move(enumerable) | std::forward<Func>(function);
}

template<EnsureIsSeq Seq, typename Func>
auto operator|(const Seq& sequence, Func&& function)
{
    return wrapAsIEnumerable(ByValue(sequence)) | std::forward<Func>(function);
}

namespace Seq
{
    inline auto chunkBySize(std::size_t size)
    {
        return [size]<typename T>(IEnumerable<T> sequence) -> IEnumerable<std::vector<T>>
        {
            return _internal::chunkBySizeNoCapture(std::move(sequence), size);
        };
    }

    template<typename Predicate>
    inline auto exists(Predicate&& pred)
    {
        return [pred = std::forward<Predicate>(pred)]<typename T>(IEnumerable<T> sequence) -> bool
        {
            for (const auto& elem : sequence)
            {
                if (pred(elem))
                {
                    return true;
                }
            }

            return false;
        };
    }

    template<typename Predicate>
    inline auto filter(Predicate&& pred)
    {
        return [pred = std::forward<Predicate>(pred)]<typename T>(IEnumerable<T> sequence) -> IEnumerable<T>
        {
            return _internal::filterNoCapture(std::move(sequence), ByValue(pred));
        };
    }

    template<typename Predicate>
    inline auto forall(Predicate&& pred)
    {
        return [pred = std::forward<Predicate>(pred)]<typename T>(IEnumerable<T> sequence) -> bool
        {
            for (const auto& elem : sequence)
            {
                if (!pred(elem))
                {
                    return false;
                }
            }

            return true;
        };
    }

    inline auto isEmpty()
    {
        return []<typename T>(IEnumerable<T> sequence) -> bool
        {
            return !(sequence.begin() != sequence.end());
        };
    }

    template<typename Action>
    inline auto iter(Action&& action)
    {
        return [action = std::forward<Action>(action)]<typename T>(IEnumerable<T> sequence) -> void
        {
            for (const auto& elem : sequence)
            {
                action(elem);
            }
        };
    }

    inline auto length()
    {
        return []<typename T>(IEnumerable<T> sequence) -> std::size_t
        {
            std::size_t length = 0;

            for ([[maybe_unused]] const auto& elem : sequence)
            {
                ++length;
            }

            return length;
        };
    }

    template<typename Mapping>
    inline auto map(Mapping&& mapping)
    {
        return [mapping = std::forward<Mapping>(mapping)]<typename T, typename U = ReturnValueOf<Mapping, T>>(
                   IEnumerable<T> sequence) -> IEnumerable<U>
        {
            return _internal::mapNoCapture<U>(std::move(sequence), ByValue(mapping));
        };
    }

    inline auto pairwise()
    {
        return []<typename T>(IEnumerable<T> sequence) -> IEnumerable<std::pair<T, T>>
        {
            std::optional<T> previousElem;

            for (const auto& elem : sequence)
            {
                if (previousElem.has_value())
                {
                    co_yield std::make_pair(*previousElem, elem);
                }

                previousElem = elem;
            }
        };
    }

    inline auto pairwiseWrap()
    {
        return []<typename T>(IEnumerable<T> sequence) -> IEnumerable<std::pair<T, T>>
        {
            bool atLeastOnePair = false;
            std::optional<T> firstElem;
            std::optional<T> previousElem;

            for (const auto& elem : sequence)
            {
                if (previousElem.has_value())
                {
                    atLeastOnePair = true;
                    co_yield std::make_pair(*previousElem, elem);
                }
                else
                {
                    firstElem = elem;
                }

                previousElem = elem;
            }

            if (atLeastOnePair)
            {
                co_yield std::make_pair(*previousElem, *firstElem);
            }
        };
    }

    inline auto range(int exclusiveMax) -> IEnumerable<int>
    {
        for (int i = 0; i < exclusiveMax; ++i)
        {
            co_yield i;
        }
    }

    template<typename Accumulator, typename Reduction>
    inline auto reduce(Accumulator&& accum, Reduction&& reduce)
    {
        return [accum = std::forward<Accumulator>(accum), reduce = std::forward<Reduction>(reduce)]<typename T>(
                   IEnumerable<T> sequence) -> Accumulator
        {
            Accumulator out = accum;

            for (const auto& elem : sequence)
            {
                out = reduce(elem, out);
            }

            return out;
        };
    }

    inline auto tail()
    {
        return []<typename T>(IEnumerable<T> sequence) -> IEnumerable<T>
        {
            std::size_t index = 0;

            for (const auto& elem : sequence)
            {
                if (index != 0)
                {
                    co_yield elem;
                }

                ++index;
            }
        };
    }

    template<std::size_t InitialReserve = 16, bool EnableShrink = false>
    inline auto toString()
    {
        return [](IEnumerable<char> sequence) -> std::string
        {
            std::string out;
            out.reserve(InitialReserve);

            for (const auto& elem : sequence)
            {
                out.append(1, elem);
            }

            if constexpr (EnableShrink)
            {
                out.shrink_to_fit();
            }

            return out;
        };
    }

    template<std::size_t InitialReserve = 16, bool EnableShrink = false>
    inline auto toVector()
    {
        return []<typename T>(IEnumerable<T> sequence) -> std::vector<T>
        {
            std::vector<T> out;
            out.reserve(InitialReserve);

            for (const auto& elem : sequence)
            {
                out.emplace_back(elem);
            }

            if constexpr (EnableShrink)
            {
                out.shrink_to_fit();
            }

            return out;
        };
    }
}
