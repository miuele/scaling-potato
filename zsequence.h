#ifndef ZSEQUENCE_H_INCLUDED
#define ZSEQUENCE_H_INCLUDED

#include <cstddef>
#include <array>

namespace zseq {

template <class T, std::size_t N>
class zsequence {
public:
    static_assert(N > 0, "N must be greater than 0");

    using value_type = T;

    template <class ...Args>
    constexpr zsequence(Args &&...args)
        : data_{std::forward<Args>(args)...}, top_(0)
    {
    }

    template <std::size_t I>
    constexpr const T &prev() const {
        static_assert(I < N, "index out of range");
        return data_[(top_ + I) % N];
    }

    constexpr const T &curr() const {
        return prev<0>();
    }

    constexpr void advance(const T &next) {
        top_ = (N + top_ - 1) % N;
        data_[top_] = next;
    }

private:
    std::array<T, N> data_;
    std::size_t top_;
};

template <class T>
class zsequence<T, 2> {
public:
    using value_type = T;

    constexpr zsequence()
        : curr_(), prev_()
    {
    }

    template <class U>
    constexpr zsequence(U &&curr)
        : curr_(std::forward<U>(curr))
    {
    }

    template <class U, class V>
    constexpr zsequence(U &&curr, V &&prev)
        : curr_(std::forward<U>(curr)), prev_(std::forward<V>(prev))
    {
    }

    template <std::size_t I>
    constexpr const T &prev() const {
        static_assert(I < 2, "index out of range");
        return I == 0 ? curr_ : prev_;
    }

    constexpr const T &prev() const {
        return prev<1>();
    }

    constexpr const T &curr() const {
        return prev<0>();
    }

    constexpr void advance(const T &next) {
        prev_ = curr_;
        curr_ = next;
    }

private:
    T curr_;
    T prev_;
};

}

#endif
