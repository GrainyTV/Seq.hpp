#include "seq/seq.hpp"
#include <iostream>

int main()
{
    auto squares =
        Seq::range(5)
        | Seq::filter([](int n) { return n % 2 == 0; })
        | Seq::map([](int n) { return n * n; });

    for (int s : squares)
        std::cout << s << " ";
}
