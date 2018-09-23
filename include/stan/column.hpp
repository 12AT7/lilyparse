#pragma once

#include <stan/value.hpp>
#include <stan/pitch.hpp>

#include <boost/hana/equal.hpp>

#include <variant>
#include <memory>

namespace stan {

struct rest
{
    BOOST_HANA_DEFINE_STRUCT(rest,
                             (value, m_value));

    rest(const value &v) :
        m_value(v) {}

    friend int operator==(rest const &, rest const &);
};

struct note
{
    BOOST_HANA_DEFINE_STRUCT(note,
                             (value, m_value),
                             (pitch, m_pitch));

    note(const value &v, const pitch &p) :
        m_value(v), m_pitch(p) {}

    friend int operator==(note const &, note const &);
};

struct chord
{
    BOOST_HANA_DEFINE_STRUCT(chord,
                             (value, m_value),
                             (std::vector<pitch>, m_pitches));

    template <typename PitchContainer>
    chord(const value &v, const PitchContainer &p) :
        m_value(v)
    {
        if (p.empty())
            throw invalid_value("chords must have at least one pitch");

        std::copy(p.begin(), p.end(), std::back_inserter(m_pitches));
        std::sort(m_pitches.begin(), m_pitches.end());
        m_pitches.erase(std::unique(m_pitches.begin(), m_pitches.end()), m_pitches.end());

        if (m_pitches.size() != p.size())
            throw invalid_value("chords must all unique pitches");
    }

    friend int operator==(chord const &, chord const &);
};

struct column;

struct tuplet
{
    BOOST_HANA_DEFINE_STRUCT(tuplet,
                             (value, m_value),
                             (std::vector<column>, m_notes));

    tuplet(const value &v, std::vector<column> &&n) :
        m_value(v), m_notes(std::move(n)) {}
    tuplet(tuplet &&) = default;
};

struct beam
{
    BOOST_HANA_DEFINE_STRUCT(beam,
                             (std::vector<column>, m_notes));

    beam(std::vector<column> &&n) :
        m_notes(std::move(n)) {}
};

using variant = std::variant<rest, note, chord, std::unique_ptr<column>>;

struct column
{
    std::variant<rest, note, chord, std::unique_ptr<column>> m_variant;

    column(const rest &v) :
        m_variant(v) {}
    column(const note &v) :
        m_variant(v) {}
    column(const chord &v) :
        m_variant(v) {}
    column(std::unique_ptr<column> &&v) :
        m_variant(std::move(v)) {}

    column(column &&) = default;

    column(variant &&v) :
        m_variant(std::move(v)) {}

    friend bool operator==(column const &, column const &);
    friend int operator==(column const &, std::unique_ptr<column> const &);
    friend bool operator==(std::unique_ptr<column> const &, std::unique_ptr<column> const &);
    friend int operator==(std::unique_ptr<column> const &, column const &);
};

} // namespace stan
