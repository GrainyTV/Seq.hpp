#include "test_imports.cpp"

TEST_CASE("Valid Range Expressions", "[ienumerable]")
{
    SECTION("C-style arrays")
    {
        STATIC_CHECK(IEnumerable<int(&)[4]>);
    }

    SECTION("Text related ranges")
    {
        STATIC_CHECK(IEnumerable<const char*>);
        STATIC_CHECK(IEnumerable<std::string>);
        STATIC_CHECK(IEnumerable<std::string_view>);
    }

    SECTION("Ranges from the standard library")
    {
        STATIC_CHECK(IEnumerable<std::array<int, 4>>);
        STATIC_CHECK(IEnumerable<std::vector<int>>);
        STATIC_CHECK(IEnumerable<std::initializer_list<int>>);
        STATIC_CHECK(IEnumerable<std::map<int, std::string>>);
        STATIC_CHECK(IEnumerable<std::unordered_map<int, std::string>>);
    }

    SECTION("Filesystem directory range")
    {
        STATIC_CHECK(IEnumerable<fs::directory_iterator>);
    }

    SECTION("Userdefined ranges that provide <begin-end> pairs")
    {
        STATIC_CHECK(IEnumerable<RegexMatchRange>);
    }
}

TEST_CASE("Invalid Range Expressions", "[ienumerable]")
{
    STATIC_CHECK_FALSE(IEnumerable<bool>);
    STATIC_CHECK_FALSE(IEnumerable<char>);
    STATIC_CHECK_FALSE(IEnumerable<int>);
    STATIC_CHECK_FALSE(IEnumerable<double>);
    STATIC_CHECK_FALSE(IEnumerable<float>);
}
