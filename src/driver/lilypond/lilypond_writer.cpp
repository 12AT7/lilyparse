#include <stan/notation.hpp>

#include <stan/driver/lilypond.hpp>
#include <stan/driver/debug.hpp>

#include <numeric>

namespace stan::lilypond {

driver::debug::writer debug;

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

    float fi = inside;
    float fo = outside;
    auto scale = rational<std::uint16_t>::quantize(fi / fo);
    return fmt::format(R"(\tuplet {}/{} {{{}}})", scale.num(), scale.den(), elements);
}

template <>
std::string writer::operator()<meter>(const meter &m) const
{
    static writer write;
    if (m.m_beats.size() == 1) {
        return fmt::format(R"(\time {}/{})", m.m_beats.front(), m.m_value.den());
    }

    std::string compound = std::accumulate(
        m.m_beats.begin(),
        m.m_beats.end(),
        std::string(),
        [v = m.m_value](std::string s, std::uint8_t beats) {
            return fmt::format("{}({} {}) ", s, beats, v.den());
        });
    compound.erase(compound.end() - 1);
    return fmt::format(R"(\compoundMeter #'({}))", compound);
}

template <>
std::string writer::operator()<clef>(const clef &c) const
{
	static std::map<clef::type, std::string> name {
		{ clef::type::treble, "treble" },
		{ clef::type::alto, "alto" },
		{ clef::type::tenor, "tenor" },
		{ clef::type::bass, "bass" },
		{ clef::type::percussion, "percussion" },
	};

	return fmt::format(R"(\clef {})", name.at(c.m_type));
}

template <>
std::string writer::operator()<key>(const key &k) const
{
    if (k == key(k.m_tonic, mode::major))
    {
	return fmt::format(R"(\key {} \major)", pitchclass_names.at(k.m_tonic));
    }

    if (k == key(k.m_tonic, mode::minor))
    {
	return fmt::format(R"(\key {} \minor)", pitchclass_names.at(k.m_tonic));
    }

    throw invalid_key("key is neither major nor minor");
}

template <>
std::string writer::operator()<column>(const column &v) const
{
    static writer write;
    return std::visit([](auto &&ev) { return write(ev); }, v);
}

} // namespace stan::lilypond
