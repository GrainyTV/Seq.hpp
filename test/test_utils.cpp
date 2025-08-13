#include "test_imports.cpp"

struct Vec2
{
    int x, y;
};

auto printVec2(Vec2 v) -> void
{
    std::cout << '(' << v.x << ", " << v.y << ")\n";
}
