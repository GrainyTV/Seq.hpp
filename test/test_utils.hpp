#pragma once
#include <iostream>

constexpr std::size_t TN = 42;

struct Vec2 { float x, y; };

class MyContainer
{
private:
    int cont[TN];

public:
    MyContainer() { for (std::size_t i = 0; i < TN; ++i) { cont[i] = i; } }

    const int* begin() const { return &cont[0]; }

    const int* end() const { return &cont[TN]; }
};

inline auto begin(const MyContainer& ctr) -> const int*
{
    return ctr.begin();
}

inline auto end(const MyContainer& ctr) -> const int*
{
    return ctr.end();
}

inline auto printChar(char c) -> void
{
    std::cout << c;
}

inline auto printVec2(Vec2 point, std::size_t i) -> void
{
    std::cout << i << " = (" << point.x << ", " << point.y << ')' << '\n';
}

inline auto isEven(std::size_t num) -> bool
{
    return num % 2 == 0;
}
