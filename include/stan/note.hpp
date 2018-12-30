#pragma once

#include <stan/column.hpp>
#include <stan/value.hpp>
#include <stan/pitch.hpp>

namespace stan {

struct note
{
    BOOST_HANA_DEFINE_STRUCT(note,
                             (value, m_value),
                             (pitch, m_pitch));

    note(const value &v, const pitch &p) :
        m_value(v), m_pitch(p) {}
};

}
