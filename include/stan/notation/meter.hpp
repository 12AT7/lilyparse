#pragma once

#include <stan/notation/value.hpp>

#include <boost/hana/define_struct.hpp>

namespace stan {

struct meter
{
    BOOST_HANA_DEFINE_STRUCT(meter,
                             (std::vector<std::uint8_t>, m_beats),
                             (value, m_value));

    meter(std::vector<std::uint8_t> beats, value v) :
        m_beats{ beats }, m_value{ v }
    {
        validate();
    }

  private:
    void validate() const;
};

}
