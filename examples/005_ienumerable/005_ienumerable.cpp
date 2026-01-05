// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃ Example: The IEnumerable Type ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
// This example showcases the main type provided by this library IEnumerable<T>. If you've programmed in C# before, you
// would think that this is an interface but here it is a coroutine class with an inner promise_type that makes it
// possible to yield values of `T` one at a time.

#include "seq/seq.hpp"
#include <iostream>

auto toSignedByte(const std::vector<char>& chunk) -> int
{
    std::string chunkStr(chunk.begin(), chunk.end());
    return (std::stoi(chunkStr) ^ 0x80) - 0x80;
}

auto toCharFromIntPair(std::pair<int, int> pair) -> char
{
    auto [a, b] = pair;
    return char(a + b - 12);
}

int main()
{
    // Say your task is to find a secret message from this numeric text.
    // Let's image you've found the encode steps on a piece of paper so you create a pipeline of operations by
    // inverting all the instructions and running them in reverse order.

    std::string encodedText =
        "06101806104807422602216321616921308"
        "70442480520660680410810340950880141"
        "01012032080043001127245120008067110";

    IEnumerable<char> decode =
        encodedText
        | Seq::chunkBySize(3)                                   // break text into 3 character chunks
        | Seq::map(toSignedByte)                                // parse each chunk as int and convert it to signed
        | Seq::tail()                                           // abandon first chunk that is randomly generated
        | Seq::filter([](int num) { return num % 22 != 0; })    // remove all multiples of 22
        | Seq::pairwise()                                       // create pairs from consecutive ints
        | Seq::map(toCharFromIntPair);                          // map pairs by adding them up and subtracting 12

    // Okay now we have the decode pipeline. There are a few interesting things to note here. You don't necessarily
    // have to use lambda functions as the callbacks. You can pass the name of a function with the expected signature
    // and it will work the same way as we can see with `toSignedByte` and `toCharFromIntPair`.

    // The other thing to note is that if your pipeline's final function returns `IEnumerable<T>` it is considered
    // lazy. Execution will be deferred until the result is materialized in some way.

    std::cout << (decode | Seq::toString()) << '\n';    // this prints "Can your C language do that?"
}
