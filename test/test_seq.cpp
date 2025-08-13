#include "test_utils.cpp"

TEST_CASE("Print An Array Of Vec2 Values", "[Seq::iter]")
{
    std::array<Vec2, 4> items = {
        Vec2({1, 2}), Vec2({3, 4}), Vec2({5, 6}), Vec2({7, 8})
    };

    SECTION("Without index")
    {
        auto stdoutBuffer = std::cout.rdbuf();
        std::ostringstream oss;
        std::cout.rdbuf(oss.rdbuf());

        items | Seq::iter(printVec2);

        std::cout.rdbuf(stdoutBuffer);
        REQUIRE(oss.str() == "(1, 2)\n(3, 4)\n(5, 6)\n(7, 8)\n");
    }

    SECTION("With index")
    {
        auto stdoutBuffer = std::cout.rdbuf();
        std::ostringstream oss;
        std::cout.rdbuf(oss.rdbuf());

        items | Seq::iteri([](std::size_t i, Vec2 elem) { std::cout << i << ": (" << elem.x << ", " << elem.y << ")\n"; });

        std::cout.rdbuf(stdoutBuffer);
        REQUIRE(oss.str() == "0: (1, 2)\n1: (3, 4)\n2: (5, 6)\n3: (7, 8)\n");
    }
}

TEST_CASE("Create Numeric Ranges", "[Seq::range]")
{
    SECTION("@Compile-time")
    {
#if __cplusplus >= 202002L
        STATIC_REQUIRE(Seq::range<4>() == std::array<int32_t, 4>({ 0, 1, 2, 3 }));
        STATIC_REQUIRE(Seq::range<1, 4>() == std::array<int32_t, 3>({ 1, 2, 3 }));
#endif
    }

    SECTION("@Runtime")
    {
        REQUIRE(Seq::range(4) == std::vector<int32_t>({ 0, 1, 2, 3 }));
        REQUIRE(Seq::range(-2, 4) == std::vector<int32_t>({ -2, -1, 0, 1, 2, 3 }));

        REQUIRE_THROWS(Seq::range(0));
        REQUIRE_THROWS(Seq::range(2, 1));
    }
}

TEST_CASE("Filter Out Items From Range", "[Seq::filter]")
{
    SECTION("Even numbers")
    {
        const auto even =
            Seq::range(10)
            | Seq::filter([](int32_t x) { return x % 2 == 0; });

        REQUIRE(even == std::vector<int32_t>({ 0, 2, 4, 6, 8 }));
        REQUIRE(even.capacity() == 10);
    }

    SECTION("Empty")
    {
        char letters[5] = { 'h', 'e', 'l', 'l', 'o' };

        const auto found =
            letters
            | Seq::filter([](char x) { return x == 'a'; });

        REQUIRE(found.empty());
        REQUIRE(found.capacity() == 5);
    }
}

TEST_CASE("Create Pairs From Arbitrary Range")
{
    uint32_t nums[4] = { 1, 2, 3, 4 };

    SECTION("Without wrapping")
    {
        const auto numPairs = nums | Seq::pairwise();
        INFO("Capacity: " << numPairs.capacity());
        REQUIRE(numPairs == std::vector<std::pair<uint32_t, uint32_t>>({ {1, 2}, {2, 3}, {3, 4} }));
    }

    SECTION("With wrapping")
    {
        const auto numPairs = nums | Seq::pairwiseWrap();
        INFO("Capacity: " << numPairs.capacity());
        REQUIRE(numPairs == std::vector<std::pair<uint32_t, uint32_t>>({ {1, 2}, {2, 3}, {3, 4}, {4, 1} }));
    }
}
