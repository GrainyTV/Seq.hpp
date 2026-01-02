#pragma once
#include <vector>
#include <format>

// ┏━━━━━━━━━━━━━┓
// ┃ std::vector ┃
// ┗━━━━━━━━━━━━━┛
template<typename T, typename CharT>
struct std::formatter<std::vector<T>, CharT>
{
    std::formatter<T, CharT> element_formatter;

    constexpr auto parse(std::basic_format_parse_context<CharT>& ctx)
    {
        return element_formatter.parse(ctx);
    }

    template<typename FormatContext>
    auto format(const std::vector<T>& vec, FormatContext& ctx) const
    {
        auto out = ctx.out();
        *out++ = CharT('[');

        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            out = element_formatter.format(vec[i], ctx);

            if (i + 1 < vec.size())
            {
                *out++ = CharT(',');
                *out++ = CharT(' ');
            }
        }

        *out++ = CharT(']');
        return out;
    }
};

// ┏━━━━━━━━━━━┓
// ┃ std::pair ┃
// ┗━━━━━━━━━━━┛
template<typename T1, typename T2, typename CharT>
struct std::formatter<std::pair<T1, T2>, CharT>
{
    std::formatter<T1, CharT> first_formatter;
    std::formatter<T2, CharT> second_formatter;

    constexpr auto parse(std::basic_format_parse_context<CharT>& ctx)
    {
        auto it = first_formatter.parse(ctx);
        second_formatter.parse(ctx);
        return it;
    }

    template<typename FormatContext>
    auto format(const std::pair<T1, T2>& p, FormatContext& ctx) const
    {
        auto out = ctx.out();

        *out++ = CharT('(');
        out = first_formatter.format(p.first, ctx);
        *out++ = CharT(',');
        *out++ = CharT(' ');
        out = second_formatter.format(p.second, ctx);
        *out++ = CharT(')');

        return out;
    }
};
