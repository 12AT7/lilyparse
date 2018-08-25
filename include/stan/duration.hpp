#pragma once

#include "rational.hpp"

namespace stan {

struct duration : rational<std::uint32_t>
{
    using rational<std::uint32_t>::rational;

    duration operator+(duration const &d2)
    {
        return { num() + d2.num(), 1 };
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
};

template <>
struct string_generator<duration>
{
    std::string operator()(duration const &d)
    {
        return std::to_string(d.num()) + "/" + std::to_string(d.den());
    }
};

} // namespace stan
