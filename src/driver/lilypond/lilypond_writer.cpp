#include <stan/driver/lilypond.hpp>

namespace stan::lilypond {

template <>
std::string writer::operator()<value>(const value& v)
{
    if (v == value::instantaneous())
    {
        return std::string();
    }

    return std::to_string(v.den() / (1u << v.dots())) + std::string(v.dots(), '.');
}

template <>
std::string writer::operator()<pitchclass>(const pitchclass& v)
{
    return pitchclass_names.at(v);
}

template <>
std::string writer::operator()<octave>(const octave& v)
{
    std::int16_t cast = static_cast<std::uint8_t>(v) - 4;
    return std::string(std::max<std::int16_t>(cast, 0), '\'') +
           std::string(-std::min<std::int16_t>(cast, 0), ',');
}

template <>
std::string writer::operator()<pitch>(const pitch& v)
{
    writer write;
    return write(v.m_pitchclass) + write(v.m_octave);
}

template <>
std::string writer::operator()<note>(const note& v)
{
    writer write;
    return write(v.m_pitch) + write(v.m_value);
}


} // stan::lilypond
