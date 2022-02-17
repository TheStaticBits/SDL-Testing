#pragma once

#include <iostream>

template<typename T>
struct Vect
{
    T x, y;

    Vect(T x, T y) : x(x), y(y) {} // Intializer
    Vect() : x(0), y(0) {} // Without parameters

    void print()
    {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
};