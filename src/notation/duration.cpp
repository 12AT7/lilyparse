#include <stan/duration.hpp>
#include <stan/value.hpp>

namespace stan {

duration operator*(int times, duration const &dur)
{
    return { times * dur.num(), dur.den() };
}

duration operator*(int times, value const &v)
{
    return times * static_cast<duration>(v);
}

} // namespace stan
