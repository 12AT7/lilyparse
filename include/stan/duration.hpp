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

    // duration(tie &&v)
    //     : duration(std::accumulate(v.begin(), v.end(), zero(),
    //           [](const duration &d, value v) { return d + v; }))
    // {
    // }

    // static duration zero()
    // {
    //     return { 0, 1 };
    // }
    //
    // private:
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

// template <>
// struct string_generator<duration>
// {
//     std::string operator()(duration const &d)
//     {
//         duration::integer gcd = d.compute_gcd(d.num(), d.den());
//         return std::to_string(d.num() / gcd) + "/" + std::to_string(d.den() / gcd);
//     }
// };

} // namespace stan
