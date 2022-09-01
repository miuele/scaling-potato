#include <iostream>

template <class T>
struct range_exclusive_both {
    T from;
    T to;
};

template <class T>
struct range_exclusive_from {
    T from;
};

template <class T>
struct range_exclusive_to {
    T to;
};

struct Infinity { explicit Infinity() = default; };
struct NegativeInfinity { explicit NegativeInfinity() = default; };

NegativeInfinity operator-(Infinity) {
    return NegativeInfinity{};
}

Infinity operator-(NegativeInfinity) {
    return Infinity{};
}

constexpr inline Infinity Inf = Infinity{};

template <class T>
struct Idx {
    Idx(T x) : value(x) {}
    Idx operator-() const { return -value; }
    operator T() const { return value; }
    T value;
};

Idx<int> operator"" _i(unsigned long long n) {
    return static_cast<int>(n);
}

template <class T>
range_exclusive_both<T> operator<(Idx<T> from, Idx<T> to) {
    return { from, to };
}

template <class T>
range_exclusive_from<T> operator<(Idx<T> from, Infinity) {
    return { from };
}

template <class T>
range_exclusive_to<T> operator<(NegativeInfinity, Idx<T> to) {
    return { to };
}

template <class T>
std::ostream &operator<<(std::ostream &os, const range_exclusive_both<T> &r) {
    os << "(" << r.from << ", " << r.to << ")";
    return os;
}

template <class T>
std::ostream &operator<<(std::ostream &os, const range_exclusive_from<T> &r) {
    os << "(" << r.from << ", inf)";
    return os;
}

template <class T>
std::ostream &operator<<(std::ostream &os, const range_exclusive_to<T> &r) {
    os << "(-inf, " << r.to << ")";
    return os;
}


int main() {
    std::cout << (-Inf < 32_i) << std::endl;
    std::cout << (-2_i < 500_i) << std::endl;
    std::cout << (2_i < Inf) << std::endl;
}
