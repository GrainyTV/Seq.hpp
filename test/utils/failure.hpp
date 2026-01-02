#pragma once
#include <exception>
#include <string>
#include <format>
#include <source_location>

using sl = std::source_location;

class Failure : public std::exception
{
private:
    std::string reason;
    std::string at;

public:
    template<typename... Args>
    explicit Failure(sl callsite, std::format_string<Args...> fmt, Args&&... args)
        : reason(std::format(fmt, std::forward<Args>(args)...))
        , at(std::format("@ {}:{}", callsite.file_name(), callsite.line()))
    {}

    const char* what() const noexcept override
    {
        return reason.c_str();
    }

    const char* where() const noexcept
    {
        return at.c_str();
    }
};
