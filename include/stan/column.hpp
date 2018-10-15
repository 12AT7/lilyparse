#pragma once

#include <stan/value.hpp>
#include <stan/pitch.hpp>
#include <stan/duration.hpp>

#include <boost/hana/equal.hpp>

#include <variant>
#include <memory>
#include <numeric>

#include <boost/variant.hpp>

namespace stan {

template <typename T>
auto default_value = T{};

template <typename T>
struct default_ctor : T
{
    default_ctor() :
        T{ stan::default_value<T> } {}

    default_ctor(const T &v) :
        T{ v }
    {
    }
};

struct rest;
struct note;
struct chord;
struct beam;
struct tuplet;
using column = std::variant<rest, note, chord, beam, tuplet>;

struct rest
{
    BOOST_HANA_DEFINE_STRUCT(rest, (value, m_value));

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

    // template <typename Pitch, template <typename> class Container>
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

    friend int operator==(chord const &, chord const &);
};

struct tuplet
{
    BOOST_HANA_DEFINE_STRUCT(tuplet,
                             (value, m_value),
                             (std::vector<column>, m_elements));

    template <typename Element>
    tuplet(const value &v, const std::vector<Element> &n) :
        m_value(v)
    {
        std::copy(n.begin(), n.end(), std::back_inserter(m_elements));
        validate();
    }

    template <typename... VoiceElement>
    tuplet(const value &v, VoiceElement... element) :
        m_value(v)
    {
        (m_elements.emplace_back(element), ...);
        validate();
    }

    static value scale(int num, int den, duration const &inner);
    static value scale(int num, int den, value const &inner);

    template <typename ElementContainer>
    static value scale(int num, int den, ElementContainer const &elements);

    friend int operator==(tuplet const &, tuplet const &);

  private:
    void validate() const;
};

struct beam
{
    BOOST_HANA_DEFINE_STRUCT(beam, (std::vector<column>, m_elements));

    template <typename Element>
    beam(const std::vector<Element> &n)
    {
        std::copy(n.begin(), n.end(), std::back_inserter(m_elements));
        validate();
    }

    template <typename... VoiceElement>
    beam(VoiceElement... element)
    {
        (m_elements.emplace_back(element), ...);
        validate();
    }

    friend int operator==(beam const &, beam const &);

  private:
    void validate() const;
};

duration operator+(const duration &d, const column &c);

#if 1
struct copy_variant
{
    template <typename T>
    column operator()(const T &v) const;

    column operator()(const tuplet &v) const;
    column operator()(const beam &v) const;
    column operator()(const column &v) const;

    template <typename... Ts>
    column operator()(const boost::variant<Ts...> &v) const;

    template <typename... Ts>
    column operator()(const std::variant<Ts...> &v) const;

    template <typename T>
    column operator()(const default_ctor<T> &v) const;
};

template <typename... Ts>
inline column copy_variant::operator()(const boost::variant<Ts...> &v) const
{
    return boost::apply_visitor(*this, v);
}

template <typename... Ts>
inline column copy_variant::operator()(const std::variant<Ts...> &v) const
{
    return std::visit(*this, v);
}

template <typename T>
inline column copy_variant::operator()(const default_ctor<T> &v) const
{
    return operator()<T>(v);
}

#endif

template <typename ElementContainer>
value tuplet::scale(int num, int den, ElementContainer const &elements)
{
    duration inner = std::accumulate(
        elements.begin(),
        elements.end(),
        duration::zero(),
        [](duration res, const auto &p) { return res + p; });

    return tuplet::scale(num, den, inner);
}

} // namespace stan
