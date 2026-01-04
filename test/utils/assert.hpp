#pragma once
#include "failure.hpp"

namespace Assert
{
    template<typename T>
    inline void equal(T have, T expected, sl callsite = sl::current())
    {
        if (have != expected)
        {
            throw Failure(callsite, "Expected {}, but found {}", expected, have);
        }
    }

    inline void truthy(bool have, sl callsite = sl::current()) { Assert::equal(have, true, callsite); }

    inline void falsey(bool have, sl callsite = sl::current()) { Assert::equal(have, false, callsite); }
}
