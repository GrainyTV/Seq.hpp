#pragma once
#include "seq/seq.hpp"

namespace IsSeqTest
{
    static_assert(EnsureIsSeq<const char*>);
    static_assert(EnsureIsSeq<std::string>);
    static_assert(EnsureIsSeq<std::vector<int>>);

    static_assert(!EnsureIsSeq<bool>);
    static_assert(!EnsureIsSeq<int>);
    static_assert(!EnsureIsSeq<double>);
}
