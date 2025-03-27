#pragma once
#include <array>
#include <numeric>
#include <vector>
#include <iterator>
#include <type_traits>
#include <algorithm>

template<typename, typename = void>
constexpr bool IEnumerable = false;

// ┏━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃ C-style String Literal ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━┛

template<std::size_t N>
constexpr bool IEnumerable<const char(&)[N]> = false;

// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃ Pointer Based Enumerable ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛

template<typename T>
constexpr bool IEnumerable<T, std::enable_if_t<
    std::is_array<std::remove_reference_t<T>>::value == false &&
    std::is_pointer<decltype(std::begin(std::declval<T>()))>::value &&
    std::is_pointer<decltype(std::end(std::declval<T>()))>::value>> = true;

// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃ Iterator Based Enumerable ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

template<typename T>
constexpr bool IEnumerable<T, std::enable_if_t<
    std::is_pointer<typename decltype(std::begin(std::declval<T>()))::pointer>::value &&
    std::is_pointer<typename decltype(std::end(std::declval<T>()))::pointer>::value>> = true;

// ┏━━━━━━━━━━━━━━━━┓
// ┃ C-style Arrays ┃
// ┗━━━━━━━━━━━━━━━━┛

template<typename T, std::size_t N>
constexpr bool IEnumerable<T(&)[N]> = true;

template<typename Func, typename... Args>
using ReturnValue = typename std::__invoke_result<Func, Args...>::type;

template<typename T, typename = std::enable_if_t<IEnumerable<T>>, typename Closure>
auto operator|(T&& sequence, Closure&& sequenceClosure) -> ReturnValue<Closure, T>
{
    return sequenceClosure(sequence);
}

template<typename T>
using ItemOf = std::decay_t<decltype(*std::begin(std::declval<const T>()))>;

template<typename Func, typename... Args>
constexpr bool IsInvokable = std::__is_invocable<Func, Args...>::value;

template<typename T1, typename T2>
using Equals = std::is_same<T1, T2>;

template<typename T>
constexpr std::size_t Len(const T& sequence)
{
    return static_cast<std::size_t>(std::distance(std::begin(sequence), std::end(sequence)));
}

template<bool Invocable, typename RetVal, typename Func, typename... Args>
struct IsValidSignatureHelper : std::false_type {};

template<typename RetVal, typename Func, typename... Args>
struct IsValidSignatureHelper<true, RetVal, Func, Args...> : Equals<RetVal, ReturnValue<Func, Args...>> {};
    
template<typename RetVal, typename Func, typename... Args>
constexpr bool IsValidSignature = IsValidSignatureHelper<IsInvokable<Func, Args...>, RetVal, Func, Args...>::value;

namespace Seq
{
    namespace _impl_details
    {
        template<typename T>
        using AtLeast32BitInteger = std::conditional_t<(sizeof(T) < sizeof(int)), int, T>;

        template<typename T, typename U = ItemOf<T>>
        std::enable_if_t<std::is_integral<U>::value, AtLeast32BitInteger<U>> sum(const T& sequence)
        {
            AtLeast32BitInteger<U> baseValue = 0;
            return std::accumulate(std::begin(sequence), std::end(sequence), baseValue);
        }

        template<typename T, typename U = ItemOf<T>>
        std::enable_if_t<std::is_same<U, float>::value, U> sum(const T& sequence)
        {
            return std::accumulate(std::begin(sequence), std::end(sequence), 0.0f);
        }

        template<typename T, typename U = ItemOf<T>>
        std::enable_if_t<std::is_same<U, double>::value, U> sum(const T& sequence)
        {
            return std::accumulate(std::begin(sequence), std::end(sequence), 0.0);
        }
    }

    template<typename Action>
    auto iter(Action&& action)
    {
        return [action](const auto& sequence) -> void
        {
            using T = ItemOf<decltype(sequence)>;
            static_assert(IsValidSignature<void, Action, const T&>, "Signature of Action should be <void(const T&)>");
            
            std::for_each(std::begin(sequence), std::end(sequence), action);
        };
    }

    template<typename Action>
    auto iteri(Action&& action)
    {
        return [action](const auto& sequence) -> void
        {
            using T = ItemOf<decltype(sequence)>;
            static_assert(IsValidSignature<void, Action, const T&, std::size_t>, "Signature of Action(I) should be <void(const T&, size_t)>");

            std::size_t i = 0;
            
            std::for_each(std::begin(sequence), std::end(sequence), [action, &i](const T& item)
            {
                action(item, i);
                ++i;
            });
        };
    }

    template<typename Predicate>
    auto filter(Predicate&& predicate)
    {
        return [predicate](const auto& sequence) -> std::vector<ItemOf<decltype(sequence)>>
        {
            using T = ItemOf<decltype(sequence)>;
            static_assert(IsValidSignature<bool, Predicate, const T&>, "Signature of Predicate should be <bool(const T&)>");

            std::vector<T> result;
            result.reserve(Len(sequence));
            
            std::copy_if(std::begin(sequence), std::end(sequence), std::back_inserter(result), predicate);
            return result;
        };
    }

    inline auto sum()
    {
        return [](const auto& sequence) -> auto
        {
            using T = ItemOf<decltype(sequence)>;
            static_assert(std::is_integral<T>::value || Equals<T, float>::value || Equals<T, double>::value, "Unsupported T type for Seq::sum");

            return _impl_details::sum(sequence);
        };
    }

    template<std::size_t N, typename Mapping>
    constexpr auto init(Mapping&& mapping)
    {
        static_assert(IsInvokable<Mapping, std::size_t>, "Signature of Mapping should be <T(size_t)>");
        using T = ReturnValue<Mapping, std::size_t>;

        std::array<std::size_t, N> indices;
        std::iota(std::begin(indices), std::end(indices), 0);

        std::array<T, N> result;
        std::transform(std::begin(indices), std::end(indices), std::begin(result), mapping);

        return result;
    }

    template<int InclusiveMin, int ExclusiveMax>
    constexpr auto range()
    {
        static_assert(InclusiveMin < ExclusiveMax, "Cannot form range from expression");

        return Seq::init<ExclusiveMax - InclusiveMin>([](std::size_t i) { return InclusiveMin + i; });
    }
}
