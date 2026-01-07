#pragma once
#include <utility>

template<typename T>
class ByValue
{
private:
    T m_value;

public:
    explicit ByValue(const T& value)
        : m_value(value)
    {}

    explicit ByValue(T&& value)
        : m_value(std::move(value))
    {}

    explicit operator T&() noexcept { return m_value; }

    ByValue(ByValue&& other) noexcept
        : m_value(other.m_value)
    {}

    ~ByValue() = default;

    ByValue(const ByValue&)                = delete;
    ByValue& operator=(const ByValue&)     = delete;
    ByValue& operator=(ByValue&&) noexcept = delete;
};
