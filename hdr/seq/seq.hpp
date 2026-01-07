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
    //static_assert(IS_INVOKABLE<Func, IEnumerable<T>>,
    //              "Right hand side of pipe operator is NOT a proper IEnumerable closure in this context");
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
    // `Seq::chunkBySize` divides the elements into chunks of the given size.
    // The last chunk may contain less elements if size was not a factor of length.
    inline auto chunkBySize(std::size_t size)
    {
        return [size]<typename T>(IEnumerable<T> sequence) -> IEnumerable<std::vector<T>>
        {
            return _internal::chunkBySizeNoCapture(std::move(sequence), size);
        };
    }

    // `Seq::exists` is a sibling function of `Seq::forall`.
    // Tests whether AT LEAST one element of the sequence satisfies the predicate.
    // Parameter pred has signature `(T) -> bool`.
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

    // `Seq::filter` returns ALL elements that pass the given predicate.
    // Parameter pred has signature `(T) -> bool`.
    template<typename Predicate>
    inline auto filter(Predicate&& pred)
    {
        return [pred = std::forward<Predicate>(pred)]<typename T>(IEnumerable<T> sequence) -> IEnumerable<T>
        {
            return _internal::filterNoCapture(std::move(sequence), ByValue(pred));
        };
    }

    // `Seq::forall` is a sibling function of `Seq::exists`.
    // Tests whether ALL elements of the sequence satisfy the predicate.
    // Parameter pred has signature `(T) -> bool`.
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

    // `Seq::isEmpty` passes in case a sequence does NOT contain any elements.
    inline auto isEmpty()
    {
        return []<typename T>(IEnumerable<T> sequence) -> bool
        {
            return !(sequence.begin() != sequence.end());
        };
    }

    // `Seq::iter` consumes a sequence by applying a "return value"-less function to each element.
    // Parameter action has signature `(T) -> void`.
    template<typename Action>
    inline auto iter(Action&& action)
    {
        return [action = std::forward<Action>(action)]<typename T>(IEnumerable<T> sequence) -> void
        {
            static_assert(IS_INVOKABLE<Action, T>);

            for (const auto& elem : sequence)
            {
                action(elem);
            }
        };
    }

    // `Seq::iteri` is equivalent to `Seq::iter` but provides an extra index parameter to use.
    // Parameter action has signature `(T, size_t) -> void`.
    template<typename Action>
    inline auto iteri(Action&& action)
    {
        return [action = std::forward<Action>(action)]<typename T>(IEnumerable<T> sequence) -> void
        {
            static_assert(IS_INVOKABLE<Action, T, std::size_t>);

            std::size_t idx = 0;

            for (const auto& elem : sequence)
            {
                action(elem, idx);
                ++idx;
            }
        };
    }

    // `Seq::length` returns the length of the sequence.
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

    // `Seq::map` applies a transformation to its elements.
    // Parameter mapping has signature `(T) -> U`.
    template<typename Mapping>
    inline auto map(Mapping&& mapping)
    {
        return [mapping = std::forward<Mapping>(mapping)]<typename T>(IEnumerable<T> sequence) -> auto
        {
            static_assert(IS_INVOKABLE<Mapping, T>);
            using U = ReturnValueOf<Mapping, T>;

            return _internal::mapNoCapture<U>(std::move(sequence), ByValue(mapping));
        };
    }

    // `Seq::mapi` is equivalent to `Seq::map` but provides an extra index parameter to use.
    // Parameter mapping has signature `(T, size_t) -> U`.
    template<typename Mapping>
    inline auto mapi(Mapping&& mapping)
    {
        return [mapping = std::forward<Mapping>(mapping)]<typename T>(IEnumerable<T> sequence) -> auto
        {
            static_assert(IS_INVOKABLE<Mapping, T, std::size_t>);
            using U = ReturnValueOf<Mapping, T, std::size_t>;

            return _internal::mapWithIndexNoCapture<U>(std::move(sequence), ByValue(mapping));
        };
    }

    // `Seq::pairwise` returns a sequence where all consecutive elements become paired.
    // e.g. `[1, 2, 3]` would become `[(1, 2), (2, 3)]`.
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

    // `Seq::pairwiseWrap` is equivalent to `Seq::pairwise` but creates an extra pair from the last and first elements.
    // e.g. `[1, 2, 3]` would become `[(1, 2), (2, 3), (3, 1)]`.
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

    // `Seq::range` returns all integers from the interval [min, max).
    // This works the same way as Python's built-in range function.
    inline auto range(int inclusiveMin, int exclusiveMax) -> IEnumerable<int>
    {
        for (int i = inclusiveMin; i < exclusiveMax; ++i)
        {
            co_yield i;
        }
    }

    // `Seq::range` returns all integers from the interval [0, max).
    // This works the same way as Python's built-in range function.
    inline auto range(int exclusiveMax) -> IEnumerable<int>
    {
        return Seq::range(0, exclusiveMax);
    }

    // `Seq::reduce` continuously applies a reduction function to the next element of the sequence and
    // an arbitrary initial value.
    // Parameter accum can have pretty much any type. Make sure it matches the return type of reduce.
    // Parameter reduce has signature `(T, typeOf[accum]) -> typeOf[accum]`.
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

    // `Seq::tail` returns all elements of the sequence EXCEPT the first one.
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
