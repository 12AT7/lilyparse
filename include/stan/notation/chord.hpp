#pragma once

#include <stan/notation/value.hpp>
#include <stan/notation/pitch.hpp>
#include <stan/exception.hpp>

#include <boost/hana/define_struct.hpp>

namespace stan {

struct invalid_chord : exception
{
    template <typename... Args>
    invalid_chord(const char *format, Args... args) :
        exception((std::string("invalid chord: ") + format).c_str(),
                  std::forward<Args>(args)...) {}
};

struct chord
{
    BOOST_HANA_DEFINE_STRUCT(chord,
                             (value, m_value),
                             (std::vector<pitch>, m_pitches));

    template <typename Container>
    chord(const value &v, Container &&n) :
        m_value(v)
    {
        if (n.size() < 2)
            throw invalid_chord("at least two pitches required");

        std::move(n.begin(), n.end(), std::back_inserter(m_pitches));
        std::sort(m_pitches.begin(), m_pitches.end());
        m_pitches.erase(std::unique(m_pitches.begin(), m_pitches.end()), m_pitches.end());

        if (m_pitches.size() != n.size())
            throw invalid_chord("unique pitches required");
    }

    template <typename... Pitch>
    chord(const value &v, pitch p1, Pitch &&... element) :
        m_value(v)
    {
        m_pitches.push_back(p1);
        (m_pitches.push_back(element), ...);

        if (m_pitches.size() < 2)
            throw invalid_chord("at least two pitches required");

        std::sort(m_pitches.begin(), m_pitches.end());
        if (std::unique(m_pitches.begin(), m_pitches.end()) != m_pitches.end())
            throw invalid_chord("unique pitches required");
    }
};

} // namespace stan 
