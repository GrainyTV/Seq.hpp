#include "test_utils.cpp"

TEST_CASE("Print A Range Of Vec2 Values", "[Seq::iter][Seq::iteri]")
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

TEST_CASE("Create Pairs From Arbitrary Range", "[Seq::pairwise][Seq::pairwiseWrap]")
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

TEST_CASE("Check Whether All Elements Satisfy Predicate", "[Seq::forall]")
{
    uint32_t nums[3] = { 6, 12, 69 };

    REQUIRE((nums | Seq::forall([](uint32_t x) { return x > 0; })));
    REQUIRE_FALSE((nums | Seq::forall([](uint32_t x) { return x % 2 == 0; })));
}

TEST_CASE("Initialize Range From Indices", "[Seq::init]")
{
    SECTION("@Compile-time")
    {
        constexpr auto firstFiveSquares = Seq::init<5>([](size_t i) { return i * i; });
        STATIC_REQUIRE(firstFiveSquares == std::array<size_t, 5>({ 0, 1, 4, 9, 16 }));
    }

    SECTION("@Runtime")
    {
        auto firstFiveSquares = Seq::init(5, [](size_t i) { return i * i; });
        REQUIRE(firstFiveSquares == std::vector<size_t>({ 0, 1, 4, 9, 16 }));
    }
}

TEST_CASE("Map A Range Of Strings To Their Length", "[Seq::map][Seq::mapi]")
{
    static constexpr std::array<std::string, 3> tokens({ "a", "bbb", "cc" });
    std::initializer_list<uint32_t> nums = { 1, 3, 5 };

    SECTION("Without index")
    {
        constexpr auto lengths = tokens | Seq::map([](const std::string& x) { return x.size(); });
        STATIC_REQUIRE(lengths == std::array<size_t, 3>({ 1, 3, 2 }));

        auto numsPlusOne = nums | Seq::map([](uint32_t x) { return x + 1; });
        REQUIRE(numsPlusOne == std::vector<uint32_t>({ 2, 4, 6 }));
    }

    SECTION("With index")
    {
        constexpr auto lengthsWithIndex = tokens | Seq::mapi([](size_t i, const std::string& x) { return i + x.size(); });
        STATIC_REQUIRE(lengthsWithIndex == std::array<size_t, 3>({ 1, 4, 4 }));

        auto numsPlusOnePlusIndex = nums | Seq::mapi([](size_t i, uint32_t x) { return x + 1 + (uint32_t) i; });
        REQUIRE(numsPlusOnePlusIndex == std::vector<uint32_t>({ 2, 5, 8 }));
    }
}

TEST_CASE("Append Range To Another Range", "[Seq::append]")
{
    SECTION("@Compile-time")
    {
        constexpr uint32_t original[3] = { 1, 2, 3 };
        constexpr std::array<uint32_t, 3> toAdd = { 6, 7, 8 };

        constexpr auto result = original | Seq::append(toAdd);
        STATIC_REQUIRE(result == std::array<uint32_t, 6>({ 1, 2, 3, 6, 7, 8 }));
    }

    SECTION("@Runtime")
    {
        uint32_t original[3] = { 1, 2, 3 };
        std::vector<uint32_t> toAdd = { 6, 7, 8 };

        auto result = original | Seq::append(toAdd);
        REQUIRE(result == std::vector<uint32_t>({ 1, 2, 3, 6, 7, 8 }));
    }
}

TEST_CASE("Sum Ranges", "[Seq::sum]")
{
    SECTION("Character")
    {
        const char* hello = "hello";
        REQUIRE((hello | Seq::sum()) == 532);
    }

    SECTION("Integer")
    {
        std::initializer_list<int> nums = { -1, 3, 69, 120, 2 };
        REQUIRE((nums | Seq::sum()) == 193);
    }

    SECTION("Floating point")
    {
        std::initializer_list<float> nums = { -1.0f, 3.14f, 2.718f };
        REQUIRE_THAT((nums | Seq::sum()), Catch::Matchers::WithinAbs(4.858f, 0.000001));
    }

    SECTION("Boolean")
    {
        std::initializer_list<bool> nums = { true, false, true };
        REQUIRE((nums | Seq::sum()) == 2);
    }
}

TEST_CASE("Wrap A Single Item In A Range")
{
    STATIC_REQUIRE(Seq::singleton(7) == std::array<int32_t, 1>({ 7 }));
    STATIC_REQUIRE(Seq::singleton('c') == std::array<char, 1>({ 'c' }));
}
