#pragma once

#include <stan/notation/value.hpp>
#include <stan/notation/pitch.hpp>

#include <boost/hana/define_struct.hpp>

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
