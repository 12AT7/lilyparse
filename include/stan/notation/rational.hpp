#pragma once

#include <stan/exception.hpp>

namespace stan {

// A note value behaves so similarly to a rational number that it was tempting
// to inherit boost::rational<> or similar base class.  However, the semantics
// of a note value are very restricted compared to a general mathematical
// number and it would have actually been more complicated to restrict the
// power of boost::rational<> than just build a new class with only valid
// operations and values defined.  A basic note value can only have numerators
// valued 1, 3, or 7 (to model 0, 1, or 2 dots, respectively) and denominators
// that are small powers of two.  Tuplets and ties complicated the situation a
// little bit, but can still be modeled as a rational number.  Tuplets and ties
// however also imply a grouping of multiple values together, so will be
// modeled as separate classes.

template <typename T>
struct rational
{
    using integer = T;

    T num() const;
    T den() const;

    void operator=(const rational &v);

    operator float() const;

    // Safety violating factory function for use in unit tests.
    static rational<T> unsafe(T n, T d);

    // Construct rational from real number.  Make it a factory function instead
    // of a constructor to avoid implicit conversion from float, and unintended
    // use.  This factory is rarely used; possibly only by tuplet.
    static rational<T> quantize(float);

  protected:
    // Make it impossible to contain an arbitrary value by allowing only
    // subclasses to construct valid values.

    rational(T n, T d)
    {
        integer gcd = compute_gcd(n, d);
        m_num = n / gcd;
        m_den = d / gcd;
    }

    static integer compute_gcd(integer a, integer b);

  private:
    T m_num;
    T m_den;
};

template <typename T>
T rational<T>::num() const
{
    return m_num;
}

template <typename T>
T rational<T>::den() const
{
    assert(m_den > 0); // Silence clang DivideZero warning
    return m_den;
}

template <typename T>
void rational<T>::operator=(rational const &v)
{
    m_num = v.num();
    m_den = v.den();
}

template <typename T>
rational<T>::operator float() const
{
    return static_cast<float>(m_num) / static_cast<float>(m_den);
}

template <typename T>
rational<T> rational<T>::unsafe(T n, T d)
{
    return rational<T>(n, d);
}

template <typename T>
bool operator<(rational<T> const &v1, rational<T> const &v2)
{
    return v1.num() * v2.den() < v2.num() * v1.den();
}

template <typename T>
bool operator>(rational<T> const &v1, rational<T> const &v2)
{
    return v1.num() * v2.den() > v2.num() * v1.den();
}

template <typename T>
bool operator>=(rational<T> const &v1, rational<T> const &v2)
{
    return v1.num() * v2.den() >= v2.num() * v1.den();
}

template <typename T>
bool operator<=(rational<T> const &v1, rational<T> const &v2)
{
    return v1.num() * v2.den() <= v2.num() * v1.den();
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

template <typename T>
T rational<T>::compute_gcd(T a, T b)
{
    while (b != 0) {
        a = std::exchange(b, a % b);
    }
    assert(a > 0); // Silence clang DivideZero warning
    return a;
}

template <typename T>
rational<T> rational<T>::quantize(float x)
{
    // https://stackoverflow.com/questions/95727/how-to-convert-floats-to-human-readable-fractions

    std::uint64_t m_00 = 1;
    std::uint64_t m_01 = 0;
    std::uint64_t m_10 = 0;
    std::uint64_t m_11 = 1;

    for (size_t i = 0; i < 7; ++i) {
        auto a = static_cast<std::uint64_t>(x);

        m_01 = std::exchange(m_00, m_00 * a + m_01);
        m_11 = std::exchange(m_10, m_10 * a + m_11);

        if (std::fabs(x - static_cast<float>(a)) < 1e-5)
            return { static_cast<T>(m_00), static_cast<T>(m_10) };

        x = 1.0f / (x - static_cast<float>(a));
    }

    // Rhythmic relationships described by tuplets are pretty simple numbers
    // compared to general mathematical rationals.  This is why the above
    // iterations are limited; no musician can play even a 13/17 tuplet, let
    // alone even bigger denominators.

    throw exception("invalid rational");
}

} // namespace stan
