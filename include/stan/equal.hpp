#pragma once

#include <boost/hana/zip.hpp>
#include <boost/hana/fold.hpp>
#include <boost/hana/members.hpp>

namespace stan {

// Hana ships with default hana::equal which should have been useful for
// defining operator== on all the hana structs.  Instead, I got major grief
// with "ambiguous" templates and other indecipherable errors.  The templates
// boost::hana::equal<> are either broken for Structs (not likely) or stan has
// violated some Hana API requirement.  Either way, I could not solve it, and
// wrote the workaround here.

template <typename S>
typename std::enable_if<boost::hana::Struct<S>::value, bool>::type
operator==(const S &c1, const S &c2)
{
    using boost::hana::at_c;
    using boost::hana::fold;
    using boost::hana::members;
    using boost::hana::zip;

    return fold(zip(members(c1), members(c2)),
                true,
                [](bool eq, auto pair) {
                    return eq && (at_c<0>(pair) == at_c<1>(pair));
                });
}

template <typename S>
typename std::enable_if<boost::hana::Struct<S>::value, bool>::type
operator!=(const S &c1, const S &c2)
{
    return !(c1 == c2);
}

} // namespace stan
