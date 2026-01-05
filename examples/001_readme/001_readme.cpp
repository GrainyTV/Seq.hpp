// ┏━━━━━━━━━━━━━━━━━━━━━━┓
// ┃ Example: Basic Usage ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━┛
// This example illustrates how to use the library at the most basic level. The library provides (mainly higher-order)
// functions in the `Seq` namespace and a few overriden `operator|` to compose them into chains.

#include "seq/seq.hpp"    // include the main header file
#include <iostream>

int main()
{
    // Here `squares` is a three component chain. You can see this easily if you count the number of `operator|` calls
    // plus one.

    // Think of the chain as this: anything that is on the left side of | will be the input to what appears on the right
    // of the |. It could be a basic function call as well like `Seq::filter(...)(Seq::range(5))` but you can see how
    // this would be pretty hard to read after like two operations.

    auto squares =
        Seq::range(5)
        | Seq::filter([](int n) { return n % 2 == 0; })
        | Seq::map([](int n) { return n * n; });

    // Now let's see what this pipeline does.

    // Seq::range(...) works the same way as range() in Python does. It returns all numbers from 0 up to (excluding) a
    // provided value.

    // Seq::filter(...) filters the input by checking a condition on each individual element and only returning those
    // that pass, in our case even numbers.

    // Seq::map(...) transforms elements from one type to another. In this case this is an (int => int) transform
    // because we just take the square of the input.

    // Here is a basic visualization:
    // Seq::range(...)  - {0, 1, 2, 3, 4}
    // Seq::filter(...) - {0, #, 2, #, 4}
    // Seq::map(...)    - {0, #, 4, #, 16}

    for (int s : squares)         // consume the sequence using a foreach loop
        std::cout << s << " ";    // this prints "0 4 16 "
}
