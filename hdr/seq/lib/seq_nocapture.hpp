#pragma once
#include "ienumerable.hpp"
#include "parameter_helpers.hpp"

#include <vector>

namespace Seq::_internal
{
    template<typename T>
    inline auto chunkBySizeNoCapture(IEnumerable<T> sequence, std::size_t size) -> IEnumerable<std::vector<T>>
    {
        std::vector<T> out;
        out.reserve(size);

        for (const auto& elem : sequence)
        {
            out.emplace_back(elem);

            if (out.size() == size)
            {
                co_yield out;
                out.clear();
            }
        }

        if (!out.empty())
        {
            co_yield out;
        }
    }

    template<typename T, typename Predicate>
    inline auto filterNoCapture(IEnumerable<T> sequence, ByValue<Predicate> pred) -> IEnumerable<T>
    {
        for (const auto& elem : sequence)
        {
            if (static_cast<Predicate>(pred)(elem))
            {
                co_yield elem;
            }
        }
    }

    template<typename RetVal, typename T, typename Mapping>
    inline auto mapNoCapture(IEnumerable<T> sequence, ByValue<Mapping> mapping) -> IEnumerable<RetVal>
    {
        for (const auto& elem : sequence)
        {
            co_yield static_cast<Mapping>(mapping)(elem);
        }
    }
}
