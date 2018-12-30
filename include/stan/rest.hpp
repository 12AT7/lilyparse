#pragma once

#include <stan/column.hpp>
#include <stan/value.hpp>

namespace stan {

struct rest
{
    BOOST_HANA_DEFINE_STRUCT(rest, (value, m_value));

    rest(const value &v) :
        m_value(v) {}
};

}
