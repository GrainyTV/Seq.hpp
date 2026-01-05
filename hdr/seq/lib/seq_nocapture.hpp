// ┏━━━━━━━━━━━━━━━━━━━┓
// ┃ seq_nocapture.hpp ┃
// ┗━━━━━━━━━━━━━━━━━━━┛
// The functions inside this file are not supposed to be called by the end user. The purpose of this file is to provide
// one more layer of abstraction to those functions that return `IEnumerable<T>` and have some sort of data parameter
// defined in their capture list. Capture parameters don't really play nice with coroutines. Even if they are value
// parameters, they will be most certainly destroyed by the time a yield happens causing them to become dangling. This
// is explained under CP.51 in the C++ Core Guidelines. To avoid this behavior we can introduce these kind of symlink
// functions tagged with `NoCapture` because parameters are copied to the coroutine frame.
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
