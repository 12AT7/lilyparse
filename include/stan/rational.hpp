#pragma once

#include <stan/exception.hpp>

#include <tuple>
#include <iostream>

namespace stan {

// A note value behaves so similarly to a rational number that it was tempting
// to inherit boost::rational<> or similar base class.  However, the semantics
// of a note value are very restricted compared to a rational and it would have
// actually been more complicated to restrict the power of boost::rational<>
// than just build a new class with only valid operations and values defined.
// A basic note value can only have numerators valued 1, 3, or 7 (to model dot
// notation) and denominators that are small powers of two.  Tuplets and ties
// complicated the situation a little bit, but can still be modeled as a
// rational number.  Tuplets and ties however also imply a grouping of multiple
// values together, so will be modeled as separate classes.

template <typename T>
struct rational
{
    using integer = T;

    T num() const { return num_; }
    T den() const { return den_; }

    rational<T> operator=(const rational &v) { return v; }

  protected:
    // Make it impossible to contain an arbitrary value by allowing only
    // subclasses to construct valid values, and nobody at all can modify the
    // values after construction.

    rational(T n, T d) :
        num_{ n }, den_{ d }
    {
        if (den_ == 0)
            throw invalid_value("zero denominator");
    }

  private:
    const T num_;
    const T den_;
};

template <typename T>
bool operator<(rational<T> const &v1, rational<T> const &v2)
{
    return v1.num() * v2.den() < v2.num() * v1.den();
}

template <typename T>
bool operator==(rational<T> const &v1, rational<T> const &v2)
{
    return v1.num() * v2.den() == v2.num() * v1.den();
}

template <typename T>
bool operator!=(rational<T> const &v1, rational<T> const &v2)
{
    return v1.num() * v2.den() != v2.num() * v1.den();
}

template <typename T>
bool operator==(rational<T> const &p1, std::pair<T, T> const &p2)
{
    return p1.num() == p2.first and p1.den() == p2.second;
}

} // namespace stan
