#pragma once

#include <stan/notation/value.hpp>

#include <boost/hana/define_struct.hpp>

namespace stan {

struct rest
{
    BOOST_HANA_DEFINE_STRUCT(rest, (value, m_value));

    rest(const value &v) :
        m_value(v) {}
};

}
