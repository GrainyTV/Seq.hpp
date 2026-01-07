#pragma once
#include "seq/seq.hpp"
#include "utils/assert.hpp"

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

#define REGISTER_TEST(fun) std::make_pair(fun, "Seq::" #fun)

namespace SeqTest
{
    static void chunkBySize()
    {
        auto firstFiveInteger = {1, 2, 3, 4, 5};

        auto everyTwoItems = firstFiveInteger | Seq::chunkBySize(2) | Seq::toVector();

        Assert::equal(everyTwoItems,
                      {
                          {1, 2},
                          {3, 4},
                          {5}
        });

        const std::string numericText = "12345";

        auto eachDigit = numericText | Seq::chunkBySize(1) | Seq::toVector();

        Assert::equal(eachDigit, {{'1'}, {'2'}, {'3'}, {'4'}, {'5'}});
    }

    static void exists()
    {
        auto firstFiveInteger = {1, 2, 3, 4, 5};

        const bool hasDividableByFour = firstFiveInteger | Seq::exists([](int n) { return n % 4 == 0; });
        Assert::truthy(hasDividableByFour);

        const bool hasDividableBySix = firstFiveInteger | Seq::exists([](int n) { return n % 6 == 0; });
        Assert::falsey(hasDividableBySix);
    }

    static void filter()
    {
        auto firstFiveInteger = {1, 2, 3, 4, 5};

        auto evenNumbers = firstFiveInteger | Seq::filter([](int n) { return n % 2 == 0; }) | Seq::toVector();

        Assert::equal(evenNumbers, {2, 4});
    }

    static void forall()
    {
        auto firstFiveInteger = {1, 2, 3, 4, 5};

        const bool onlyHasEven = firstFiveInteger | Seq::forall([](int n) { return n % 2 == 0; });
        Assert::falsey(onlyHasEven);

        const bool largerThanZero = firstFiveInteger | Seq::forall([](int n) { return n > 0; });
        Assert::truthy(largerThanZero);
    }

    static void isEmpty()
    {
        const std::initializer_list<int> emptyInitializer = {};
        const bool isZeroLength                           = emptyInitializer | Seq::isEmpty();

        Assert::truthy(isZeroLength);

        auto firstFiveInteger      = {1, 2, 3, 4, 5};
        const bool isNotZeroLength = firstFiveInteger | Seq::isEmpty();

        Assert::falsey(isNotZeroLength);
    }

    static void length()
    {
        const std::initializer_list<int> emptyInitializer = {};
        const std::size_t zeroLength                      = emptyInitializer | Seq::length();

        Assert::equal<std::size_t>(zeroLength, 0ul);

        auto firstFiveInteger        = {1, 2, 3, 4, 5};
        const std::size_t fiveLength = firstFiveInteger | Seq::length();

        Assert::equal<std::size_t>(fiveLength, 5ul);
    }

    static void map()
    {
        const std::vector<std::string> fruits{"Apple", "Banana", "Orange", "Watermelon"};

        // Without index

        {
            auto fruitLengths =
                fruits
                | Seq::map([](const std::string& fruit) { return fruit.size(); })
                | Seq::toVector();

            Assert::equal(fruitLengths, {5, 6, 6, 10});

            auto fruitSlices =
                fruits
                | Seq::map([](const std::string& fruit) { return fruit.substr(1, 3); })
                | Seq::toVector();

            Assert::equal(fruitSlices, {"ppl", "ana", "ran", "ate"});
        }

        // With index

        {
            auto fruitLengths =
                fruits
                | Seq::mapi([](const std::string& fruit, std::size_t idx) { return fruit.size() + idx; })
                | Seq::toVector();

            Assert::equal(fruitLengths, {5, 7, 8, 13});
        }
    }

    static void pairwise()
    {
        auto firstFiveInteger = {1, 2, 3, 4, 5};

        auto integerPairs = firstFiveInteger | Seq::pairwise() | Seq::toVector();

        Assert::equal(integerPairs,
                      {
                          {1, 2},
                          {2, 3},
                          {3, 4},
                          {4, 5}
        });

        auto onlyOneInteger = {0};

        auto noPairs = onlyOneInteger | Seq::pairwise() | Seq::toVector();

        Assert::equal(noPairs, {});
    }

    static void pairwiseWrap()
    {
        auto firstFiveInteger = {1, 2, 3, 4, 5};

        auto integerPairs = firstFiveInteger | Seq::pairwiseWrap() | Seq::toVector();

        Assert::equal(integerPairs,
                      {
                          {1, 2},
                          {2, 3},
                          {3, 4},
                          {4, 5},
                          {5, 1}
        });

        auto onlyOneInteger = {0};

        auto noPairs = onlyOneInteger | Seq::pairwiseWrap() | Seq::toVector();

        Assert::equal(noPairs, {});

        auto firstTwoInteger = {1, 2};

        auto backToBackPairs = firstTwoInteger | Seq::pairwiseWrap() | Seq::toVector();

        Assert::equal(backToBackPairs,
                      {
                          {1, 2},
                          {2, 1}
        });
    }

    static void range()
    {
        Assert::equal(Seq::range(5) | Seq::toVector(), {0, 1, 2, 3, 4});
        Assert::equal(Seq::range(1, 5) | Seq::toVector(), {1, 2, 3, 4});
    }

    static void reduce()
    {
        auto firstFiveInteger = {1, 2, 3, 4, 5};

        auto intsWithCharacters =
            firstFiveInteger
            | Seq::reduce(std::unordered_map<int, char>{}, [](int n, auto map) {
                    map.emplace(n, 'A' + n);
                    return map;
                });

        Assert::equal(intsWithCharacters, {{1, 'B'}, {2, 'C'}, {3, 'D'}, {4, 'E'}, {5, 'F'}});

        const std::string fruitText = "\napple\n\nbanana\npear\nwatermelon";

        auto splitTextByNewline =
            fruitText
            | Seq::reduce(std::pair<std::vector<std::string>, std::string>{}, [](char chr, auto& accum) {
                    std::vector<std::string>& out = accum.first;
                    std::string& current          = accum.second;

                    if (chr == '\n' && !current.empty())
                    {
                        out.emplace_back(current);
                        current.clear();

                        return std::make_pair(out, current);
                    }

                    return std::make_pair(out, chr == '\n' ? current : current + chr);
                });

        Assert::equal(splitTextByNewline, {{"apple", "banana", "pear"}, "watermelon"});
    }

    static void tail()
    {
        auto firstFiveInteger = {1, 2, 3, 4, 5};

        auto firstFiveIntegerTail = firstFiveInteger | Seq::tail() | Seq::toVector();

        Assert::equal(firstFiveIntegerTail, {2, 3, 4, 5});

        auto onlyOneInteger = {0};

        auto noTail = onlyOneInteger | Seq::tail() | Seq::toVector();

        Assert::equal(noTail, {});
    }

    constexpr std::array CASES = {
        REGISTER_TEST(chunkBySize),
        REGISTER_TEST(exists),
        REGISTER_TEST(filter),
        REGISTER_TEST(forall),
        REGISTER_TEST(isEmpty),
        REGISTER_TEST(length),
        REGISTER_TEST(map),
        REGISTER_TEST(pairwise),
        REGISTER_TEST(pairwiseWrap),
        REGISTER_TEST(range),
        REGISTER_TEST(reduce),
        REGISTER_TEST(tail),

        // register new test cases here ...
    };
}
