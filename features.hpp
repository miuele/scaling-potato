#include <iostream>

template <class T>
struct S {
    void f(const T &);
};

template <class T>
S<T>::f(const T &) {
    std::cout << "hello" << std::endl;
}
