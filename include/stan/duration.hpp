#pragma once

#include "rational.hpp"

namespace stan {

// Durations model musical time, and represent sums of note values.

struct duration : rational<std::uint32_t>
{
    using rational<std::uint32_t>::rational;

    friend duration operator+(duration const &d1, duration const &d2);
    friend duration operator*(int, duration const &);

    // Arbitrary durations can be constructed in very few places, as a static
    // safety feature.  Normally they are queried from objects, accumulated
    // with operator+ or operator*, or initialized with duration::zero().

    static duration zero() { return duration(0, 1); }
    friend struct value;
    friend struct tuplet;
};

} // namespace stan
