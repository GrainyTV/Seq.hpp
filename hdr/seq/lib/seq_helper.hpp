#pragma once
#include "ienumerable.hpp"
#include "parameter_helpers.hpp"
#include "type_inspect_utils.hpp"

namespace Seq::_internal
{
    using TypeInspect::ItemOf;

    template<typename Seq>
    IEnumerable<ItemOf<Seq>> wrapAsIEnumerable(ByValue<Seq> sequence)
    {
        for (const auto& elem : static_cast<Seq>(sequence))
        {
            co_yield elem;
        }
    }

    template<typename T, typename Accum>
    auto sum(IEnumerable<T> sequence, Accum accum) -> Accum
    {
        for (const auto& elem : sequence)
        {
            accum += elem;
        }

        return accum;
    }

    template<typename T>
    auto rangeIncreasing(T inclusiveMin, T exclusiveMax, T step) -> IEnumerable<T>
    {
        for (T i = inclusiveMin; i < exclusiveMax; i += step)
        {
            co_yield i;
        }
    }

    template<typename T>
    auto rangeDecreasing(T inclusiveMin, T exclusiveMax, T step) -> IEnumerable<T>
    {
        for (T i = inclusiveMin; i > exclusiveMax; i += step)
        {
            co_yield i;
        }
    }
}
