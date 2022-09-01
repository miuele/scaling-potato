#include <cstddef>
#include <initializer_list>
#include <array>

#include <utility>
#include <iostream>
#include <cstdio>

namespace zseq {

template <class T, std::size_t N>
class zsequence {
public:
    static_assert(N > 0, "N must be greater than 0");

    using value_type = T;

    constexpr zsequence()
        : i_(0), data_{}
    {
    }

    template <class InputIt>
    constexpr zsequence(InputIt first, InputIt last)
        : i_(0)
    {
        auto it = first;
        std::size_t i = 0;
        for (; i < N && it != last; ++i, ++it) {
            data_[i] = *it;
        }
        for (; i < N; ++i) {
            data_[i] = T{};
        }
    }

    constexpr zsequence(std::initializer_list<T> init)
        : zsequence(std::begin(init), std::end(init))
    {
    }

    template <std::size_t I>
    constexpr const T &prev() const {
        static_assert(I < N, "index out of range");
        return data_[(i_ + I) % N];
    }

    constexpr const T &curr() const {
        return prev<0>();
    }

    constexpr void advance(const T &next) {
        i_ = (N + i_ - 1) % N;
        data_[i_] = next;
    }

private:
    std::array<T, N> data_;
    std::size_t i_;
};

template <class T>
class zsequence<T, 2> {
public:
    using value_type = T;

    constexpr zsequence()
        : curr_(), prev_()
    {
    }

    template <class InputIt>
    constexpr zsequence(InputIt first, InputIt last)
    {
        auto dist = std::distance(first, last);
        if (dist == 0) {
            curr_ = T{};
            prev_ = T{};
        } else if (dist == 1) {
            curr_ = *first;
            prev_ = T{};
        } else {
            curr_ = *first++;
            prev_ = *first;
        }
    }

    constexpr zsequence(std::initializer_list<T> init)
        : zsequence(std::begin(init), std::end(init))
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

using namespace zseq;

template <class T, std::size_t N, std::size_t ...Seq>
void print_rest(const zsequence<T, N> &seq, std::integer_sequence<std::size_t, Seq...>) {
    ((std::cout << ", " << seq.template prev<1 + Seq>()), ...);
    std::cout << '\n';
}

template <class T, std::size_t N>
void print(const zsequence<T, N> &seq) {
    std::cout << seq.template prev<0>();
    print_rest(seq, std::make_integer_sequence<std::size_t, N - 1>());
}

template <class T, std::size_t M, std::size_t N, class U>
void advance_trapezoidal_diff(zsequence<T, M> &diff, const zsequence<T, N> &seq, U h) {
    diff.advance(2 * (seq.curr() - seq.template prev<1>()) / h - diff.curr());
}

template <class T, std::size_t M, std::size_t N, class U>
void advance_forward_diff(zsequence<T, M> &diff, const zsequence<T, N> &seq, U h) {
    diff.advance((seq.curr() - seq.prev()) / h);
}

int main() {
    constexpr float h = 0.2;

    zsequence<float, 2> seq = {-0.05, 0};
    zsequence<float, 2> vel_trap = {0.25, 0};
    zsequence<float, 2> vel_forw = {0.25, 0};

    for (;;) {
        float x;
        std::scanf("%f", &x);
        seq.advance(x);
        advance_trapezoidal_diff(vel_trap, seq, h);
        advance_forward_diff(vel_forw, seq, h);
        std::printf("%f,%f\n", vel_trap.curr(), vel_forw.curr());
    }
}
