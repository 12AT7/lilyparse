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

// Because voice elements can nest, such as notes and chords being elements of
// beams and tuplets in addition to valid voice elements by themselves, the
// std::variant<> modeling voice elements must be able to contain other
// instances of itself.  In the Boost days, this was done using a class
// boost::recursive_variant<>.  With std::variant<>, we have to put nested
// instances in a std::unique_ptr<column> as a basic requirement that in staff
// music notation columns can nest, but in std::variant<> they cannot.  Not a
// big deal, except that std::unique_ptr<column> is *move only*.
//
// This ultimately results in column being move-only, causing intense pain in
// the construction and copying of these objects, with Spirit X3 (in
// lilypond_reader.cpp) and the C++ programmer both sad and howling in agony.
//
// Well, at least the implementation should be fast...

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
        // std::cout << "construct ctor " << typeid(T).name() << std::endl;
    }
};

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

    template <typename Container>
    chord(const value &v, Container &&n) :
        m_value(v)
    {
        std::move(n.begin(), n.end(), std::back_inserter(m_pitches));
        if (m_pitches.size() < 2)
            throw invalid_value("chord must have at least two elements");
    }

    template <typename... Pitch>
    chord(const value &v, Pitch &&... element) :
        m_value(v)
    {
        (m_pitches.push_back(element), ...);
        if (m_pitches.size() < 2)
            throw invalid_value("chords must have at least two pitches");
    }

    friend int operator==(chord const &, chord const &);
};

struct column;

struct tuplet
{
    BOOST_HANA_DEFINE_STRUCT(tuplet,
                             (value, m_value),
                             (std::vector<column>, m_elements));

    template <typename Container>
    tuplet(const value &v, Container &&n) :
        m_value(v)
    {
        std::move(n.begin(), n.end(), std::back_inserter(m_elements));
        if (m_elements.size() < 2)
            throw invalid_value("tuplets must have at least two elements");
    }

    template <typename... VoiceElement>
    tuplet(const value &v, VoiceElement &&... element) :
        m_value(v)
    {
        (m_elements.push_back(element), ...);
        if (m_elements.size() < 2)
            throw invalid_value("tuplets must have at least two elements");
    }

    friend int operator==(tuplet const &, tuplet const &);

    static value scale(int num, int den, duration const &inner);
    static value scale(int num, int den, value const &inner);

    template <typename ElementContainer>
    static value scale(int num, int den, ElementContainer const &elements);
};

struct beam
{
    BOOST_HANA_DEFINE_STRUCT(beam, (std::vector<column>, m_elements));

    template <typename ElementContainer>
    beam(const ElementContainer &n)
    {
        std::move(n.begin(), n.end(), std::back_inserter(m_elements));
        validate();
    }

    template <typename... VoiceElement>
    beam(VoiceElement &&... element)
    {
        (m_elements.emplace_back(element), ...);
        validate();
    }

    friend int operator==(beam const &, beam const &);
    void validate() const;
};

using variant = std::variant<rest, note, chord, beam, tuplet, std::unique_ptr<column>>;

struct copy_variant
{
    template <typename T>
    variant operator()(const T &v) const;

    variant operator()(const std::unique_ptr<column> &v) const;

    variant operator()(const tuplet &v) const;
    variant operator()(const beam &v) const;
    variant operator()(const column &v) const;

    template <typename... Ts>
    variant operator()(const boost::variant<Ts...> &v) const;

    template <typename... Ts>
    variant operator()(const std::variant<Ts...> &v) const;

    template <typename T>
    variant operator()(const default_ctor<T> &v) const;
};

struct column
{
    variant m_variant;

    column(const rest &v) :
        m_variant(v) {}
    column(const note &v) :
        m_variant(v) {}
    column(const chord &v) :
        m_variant(v) {}
    // column(const beam &v) :
    //     m_variant(copy_variant()(v)) {}
    column(const tuplet &v) :
        m_variant(copy_variant()(v)) {}
    column(std::unique_ptr<column> &&v) :
        m_variant(std::move(v)) {}
    explicit column(variant &&v) :
        m_variant{ std::move(v) } {}

    column(const column &c);
    void operator=(const column &c);

    friend bool operator==(column const &, column const &);
    friend int operator==(column const &, std::unique_ptr<column> const &);
    friend bool operator==(std::unique_ptr<column> const &, std::unique_ptr<column> const &);
    friend int operator==(std::unique_ptr<column> const &, column const &);
};

duration operator+(const duration &d, const column &c);

template <typename... Ts>
inline variant copy_variant::operator()(const boost::variant<Ts...> &v) const
{
    return boost::apply_visitor(*this, v);
}

template <typename... Ts>
inline variant copy_variant::operator()(const std::variant<Ts...> &v) const
{
    return std::visit(*this, v);
}

template <typename T>
inline variant copy_variant::operator()(const default_ctor<T> &v) const
{
    return operator()<T>(v);
}

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
