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

    chord(const value &v, const std::vector<pitch> &p) :
        m_value(v), m_pitches(p) {}

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

    // template <typename T>
    // column(T &&value) :
    //     m_variant(value) {}

    column(column &&) = default;
    // column(const column &) = default;
    // column &operator=(const column &col);

    column(variant &&v) :
        m_variant(std::move(v)) {}

    friend bool operator==(column const &, column const &);
    friend int operator==(column const &, std::unique_ptr<column> const &);
    friend bool operator==(std::unique_ptr<column> const &, std::unique_ptr<column> const &);
    friend int operator==(std::unique_ptr<column> const &, column const &);
};

} // namespace stan
