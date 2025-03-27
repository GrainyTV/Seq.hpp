#include "seq/seq.hpp"
#include "test_utils.hpp"

#include <array>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <string_view>

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_string.hpp"

namespace CM = Catch::Matchers;

TEST_CASE("Valid Sequence Expressions")
{
    REQUIRE(IEnumerable<std::array<int, TN>>);
    REQUIRE(IEnumerable<std::vector<int>>);
    REQUIRE(IEnumerable<std::map<int, double>>);
    REQUIRE(IEnumerable<std::string>);
    REQUIRE(IEnumerable<char(&)[TN]>);
    REQUIRE(IEnumerable<MyContainer>);
}

TEST_CASE("Invalid Sequence Expressions")
{
    struct A {};
    struct OnlyBegin { char* begin(); };
    struct OnlyEnd { char* end(); };

    REQUIRE(IEnumerable<int> == false);
    REQUIRE(IEnumerable<double> == false);
    REQUIRE(IEnumerable<A> == false);
    REQUIRE(IEnumerable<OnlyBegin> == false);
    REQUIRE(IEnumerable<OnlyEnd> == false);
    REQUIRE(IEnumerable<const char(&)[TN]> == false);
}

TEST_CASE("Iterate Over Elements Of Sequence", "[Seq::iter]")
{
    auto stdout = std::cout.rdbuf();
    std::ostringstream oss;
    std::cout.rdbuf(oss.rdbuf());

    SECTION("String L-value")
    {
        std::string text = "Hello!";
        text | Seq::iter(printChar);
        std::cout.rdbuf(stdout);
        REQUIRE_THAT(oss.str(), CM::Equals("Hello!"));
    }

    SECTION("C-style Array")
    {
        char tokens[] = { 'H', 'e', 'l', 'l', 'o', '!' };
        tokens | Seq::iter(printChar);
        std::cout.rdbuf(stdout);
        REQUIRE_THAT(oss.str(), CM::Equals("Hello!"));
    }

    SECTION("Raw String Literal")
    {
        using std::literals::string_view_literals::operator""sv;

        "Hello!"sv | Seq::iter(printChar);
        std::cout.rdbuf(stdout);
        REQUIRE_THAT(oss.str(), CM::Equals("Hello!"));
    }
}

TEST_CASE("Iterate Over Elements Of Sequence With Index", "[Seq::iteri]")
{
    auto stdout = std::cout.rdbuf();
    std::ostringstream oss;
    std::cout.rdbuf(oss.rdbuf());

    Vec2 square[] = { Vec2({0, 0}), Vec2({1, 0}), Vec2({1, 1}), Vec2({0, 1}) };
    square | Seq::iteri(printVec2);
    std::cout.rdbuf(stdout);
    REQUIRE_THAT(oss.str(), CM::Equals("0 = (0, 0)\n1 = (1, 0)\n2 = (1, 1)\n3 = (0, 1)\n"));
}

TEST_CASE("Filter Out Even Numbers", "[Seq::filter]")
{
    size_t input[] = { 1, 2, 3, 4, 5 };
    const auto result = input | Seq::filter(isEven);
    REQUIRE(result == std::vector<size_t>({ 2, 4 }));
}

TEST_CASE("Sum Sequences", "[Seq::sum]")
{
    SECTION("Positive Integers")
    {
        size_t input[] = { 1, 5, 3, 2 };
        const size_t sum = input | Seq::sum();
        REQUIRE(sum == 11);
    }

    SECTION("Text")
    {
        std::string input = "abcde";
        REQUIRE((input | Seq::sum()) == 495);
    }

    SECTION("Custom Container")
    {
        MyContainer myCont;
        std::size_t sum = myCont | Seq::sum();
        REQUIRE(sum == 861);
    }
}
