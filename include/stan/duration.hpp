#pragma once

#include "rational.hpp"

namespace stan {

struct duration : rational<std::uint32_t>
{
    using rational<std::uint32_t>::rational;

    duration operator+(duration const &d2)
    {
        // https://www.geeksforgeeks.org/program-to-add-two-fractions
        integer gcd = compute_gcd(den(), d2.den());
        integer d = den() * d2.den() / gcd;
        integer n = num() * (d / den()) + d2.num() * (d / d2.den());
        return { n, d };
    }

    friend struct value;

  private:
    integer compute_gcd(integer a, integer b) const
    {
        while (b != 0) {
            integer tmp = b;
            b = a % b;
            a = tmp;
        }
        return a;
    }
};

} // namespace stan
