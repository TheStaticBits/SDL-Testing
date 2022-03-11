#pragma once

#include <iostream>

template<typename T>
struct Vect
{
    T x, y;

    Vect(T x, T y) : x(x), y(y) {} // Intializer
    Vect() : x(0), y(0) {} // Without parameters

    // Copy constructor
    Vect(const Vect<T>& other) { x = other.x; y = other.y; }
    // Copy assignment operator
    Vect<T>& operator=(const Vect<T>& other) { x = (T)other.x; y = (T)other.y; return *this; }

    // Other operators
    bool operator!=(const Vect<T>& other) { return x != other.x || y != other.y; }
    bool operator==(const Vect<T>& other) { return x == other.x && y == other.y; }

    void print() const
    {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
};