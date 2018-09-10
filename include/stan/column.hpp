#pragma once

#include <stan/value.hpp>
#include <stan/pitch.hpp>

#include <variant>
#include <memory>

namespace stan {

struct rest
{
    BOOST_HANA_DEFINE_STRUCT(rest,
            (value, m_value)
            );

    rest(const value& v) : m_value(v) {}
};

struct note
{
    BOOST_HANA_DEFINE_STRUCT(note,
            (value, m_value),
            (pitch, m_pitch)
            );

    note(const value& v, const pitch& p) : m_value(v), m_pitch(p) {}
};

struct chord
{
    BOOST_HANA_DEFINE_STRUCT(chord,
            (value, m_value),
            (std::vector<pitch>, m_pitches)
            );

    chord(const value& v, const std::vector<pitch>& p) : m_value(v), m_pitches(p) {}
};

struct column;

struct tuplet
{
    BOOST_HANA_DEFINE_STRUCT(tuplet,
            (value, m_value),
            (std::vector<column>, m_notes)
            );

    tuplet(const value& v, std::vector<column>&& n) : m_value(v), m_notes(std::move(n)) {}
    tuplet(tuplet&&) = default;
};

struct beam
{
    BOOST_HANA_DEFINE_STRUCT(beam,
            (std::vector<column>, m_notes)
            );

    beam(std::vector<column>&& n) : m_notes(std::move(n)) {}
};

struct column
{
    std::variant<rest, note, chord, std::unique_ptr<column>> m_variant;

    template <typename...Args>
    column(Args&&...args) : m_variant(adapt(std::forward<Args>(args))...) {}

    column(column&&) = default;

 private:

    // Provide an adapter for syntatical sugar when constructing with nested
    // columns that requires using std::unique_ptr<column> instead of the bare
    // column.
    template <typename T>
    static auto adapt(T&& elem) { return elem; }
};

template <>
auto column::adapt<tuplet>(tuplet&& elem)
{
    return std::make_unique<tuplet>(std::move(elem));
}

} // namespace stan
