#include <stan/duration.hpp>
#include <stan/value.hpp>

namespace stan {

duration operator+(duration const &d1, duration const &d2)
{
    using integer = duration::integer;

    integer d1_den = d1.den();
    integer d2_den = d2.den();
    integer gcd = duration::compute_gcd(d1_den, d2_den);

    // Silence Division by Zero check
    assert(d1_den > 0);
    assert(d2_den > 0); // Silence Division by Zero check
    assert(gcd > 0);    // Silence Division by Zero check

    // https://www.geeksforgeeks.org/program-to-add-two-fractions
    integer d = (d1_den * d2_den) / gcd;
    integer n = (d1.num() * d) / d1_den + (d2.num() * d) / d2_den;
    return { n, d };
}

duration operator*(int times, duration const &dur)
{
    return { times * dur.num(), dur.den() };
}

duration operator*(int times, value const &v)
{
    return times * static_cast<duration>(v);
}

} // namespace stan
