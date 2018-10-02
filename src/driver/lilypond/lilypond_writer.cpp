#include <stan/driver/lilypond.hpp>
#include <stan/duration.hpp>

#include <numeric>

namespace stan::lilypond {

template <>
std::string writer::operator()<value>(const value &v) const
{
    if (v == value::instantaneous()) {
        return std::string();
    }

    return std::to_string(v.den() / (1u << v.dots())) + std::string(v.dots(), '.');
}

template <>
std::string writer::operator()<pitchclass>(const pitchclass &v) const
{
    return pitchclass_names.at(v);
}

template <>
std::string writer::operator()<octave>(const octave &v) const
{
    std::int16_t cast = static_cast<std::uint8_t>(v) - 4;
    return std::string(std::max<std::int16_t>(cast, 0), '\'') +
        std::string(-std::min<std::int16_t>(cast, 0), ',');
}

template <>
std::string writer::operator()<pitch>(const pitch &v) const
{
    writer write;
    return write(v.m_pitchclass) + write(v.m_octave);
}

template <>
std::string writer::operator()<rest>(const rest &v) const
{
    writer write;
    return fmt::format("r{}", write(v.m_value));
}

template <>
std::string writer::operator()<note>(const note &v) const
{
    writer write;
    return write(v.m_pitch) + write(v.m_value);
}

template <>
std::string writer::operator()<chord>(chord const &r) const
{
    static writer write;

    std::string pitches = std::accumulate(
        r.m_pitches.begin(),
        r.m_pitches.end(),
        std::string(),
        [](std::string res, pitch p) { return res + write(p) + " "; });
    pitches.resize(pitches.size() - 1);
    return fmt::format("<{}>{}", pitches, write(r.m_value));
}

template <>
std::string writer::operator()<beam>(beam const &r) const
{
    static writer write;

    std::string elements = std::accumulate(
        r.m_elements.begin(),
        r.m_elements.end(),
        std::string(),
        [](std::string res, const auto &p) { return res + write(p) + " "; });
    elements.resize(elements.size() - 1);
    return fmt::format("[{}]", elements);
}

template <>
std::string writer::operator()<tuplet>(tuplet const &r) const
{
    static writer write;

    std::string elements = std::accumulate(
        r.m_elements.begin(),
        r.m_elements.end(),
        std::string(),
        [](std::string res, const auto &p) { return res + write(p) + " "; });
    elements.resize(elements.size() - 1);

    duration inside = std::accumulate(
        r.m_elements.begin(),
        r.m_elements.end(),
        duration::zero(),
        [](duration res, const auto &p) { return res + p; });

    duration outside = r.m_value;

    int top = inside.num() / outside.den();
    int bottom = inside.den() / outside.num();

    return fmt::format(R"(\tuplet {}/{} {{{}}})", top, bottom, elements);
}

template <>
std::string writer::operator()<std::unique_ptr<column>>(const std::unique_ptr<column> &v) const
{
    static writer write;
    return std::visit([](auto &&ev) { return write(ev); }, v->m_variant);
}

template <>
std::string writer::operator()<column>(const column &v) const
{
    static writer write;
    return std::visit([](auto &&ev) { return write(ev); }, v.m_variant);
}

} // namespace stan::lilypond
