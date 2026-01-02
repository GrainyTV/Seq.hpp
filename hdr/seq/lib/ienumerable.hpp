#pragma once
#include <coroutine>
#include <utility>

template<typename T>
class IEnumerable
{
public:
    // NOLINTBEGIN(readability-identifier-naming): Promise type is expected to use snake case by C++ standard
    class promise_type
    {
    public:
        using Handle = std::coroutine_handle<promise_type>;

        auto get_return_object() { return IEnumerable(Handle::from_promise(*this)); }

        std::suspend_always initial_suspend() noexcept { return {}; }

        std::suspend_always final_suspend() noexcept { return {}; }

        void unhandled_exception() {}

    private:
        T currentValue;

    public:
        std::suspend_always yield_value(T expr)
        {
            currentValue = std::move(expr);
            return {};
        }

        const T& unwrap() const { return currentValue; }
    };

    // NOLINTEND(readability-identifier-naming)

private:
    class IEnumerator
    {
    private:
        promise_type::Handle ienumeratorHandle;

    public:
        void operator++() { ienumeratorHandle.resume(); }

        const T& operator*() const { return ienumeratorHandle.promise().unwrap(); }

        bool operator!=(const IEnumerator& /*unused*/) const noexcept
        {
            return ienumeratorHandle.address() != nullptr && !ienumeratorHandle.done();
        }

        explicit IEnumerator(const promise_type::Handle handle)
            : ienumeratorHandle(handle)
        {
        }

        IEnumerator()
            : ienumeratorHandle({})
        {
        }
    };

    promise_type::Handle ienumerableHandle;

    explicit IEnumerable(const promise_type::Handle handle)
        : ienumerableHandle(handle)
    {
    }

public:
    IEnumerator begin() const
    {
        if (ienumerableHandle.address() != nullptr && !ienumerableHandle.done())
        {
            ienumerableHandle.resume();
        }

        return IEnumerator(ienumerableHandle);
    }

    IEnumerator end() const { return {}; }

    IEnumerable(IEnumerable&& other) noexcept
        : ienumerableHandle(other.ienumerableHandle)
    {
        other.ienumerableHandle = {};
    }

    ~IEnumerable()
    {
        if (ienumerableHandle)
        {
            ienumerableHandle.destroy();
        }
    }

    IEnumerable(const IEnumerable&)                = delete;
    IEnumerable& operator=(const IEnumerable&)     = delete;
    IEnumerable& operator=(IEnumerable&&) noexcept = delete;
};
