#pragma once
#include <regex>
#include <string>

class RegexMatchRange
{
private:
    std::string _content;
    std::regex _regex;
    std::sregex_iterator _begin;
    std::sregex_iterator _end;

public:
    RegexMatchRange(const std::string& content, const std::string& pattern)
        : _content(content)
        , _regex(pattern)
        , _begin(_content.begin(), _content.end(), _regex)
        , _end()
    {}

    const std::sregex_iterator& begin() const
    {
        return _begin;
    }

    const std::sregex_iterator& end() const
    {
        return _end;
    }
};
