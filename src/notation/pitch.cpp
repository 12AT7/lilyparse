#include <stan/exception.hpp>
#include <stan/pitch.hpp>

#include <algorithm>
#include <map>

namespace stan {

const std::map<pitchclass, const char *> pitchclass_names = {
    { pitchclass::cff, "cff" },
    { pitchclass::cf, "cf" },
    { pitchclass::c, "c" },
    { pitchclass::cs, "cs" },
    { pitchclass::css, "css" },
    { pitchclass::dff, "dff" },
    { pitchclass::df, "df" },
    { pitchclass::d, "d" },
    { pitchclass::ds, "ds" },
    { pitchclass::dss, "dss" },
    { pitchclass::eff, "eff" },
    { pitchclass::ef, "ef" },
    { pitchclass::e, "e" },
    { pitchclass::es, "es" },
    { pitchclass::ess, "ess" },
    { pitchclass::fff, "fff" },
    { pitchclass::ff, "ff" },
    { pitchclass::f, "f" },
    { pitchclass::fs, "fs" },
    { pitchclass::fss, "fss" },
    { pitchclass::gff, "gff" },
    { pitchclass::gf, "gf" },
    { pitchclass::g, "g" },
    { pitchclass::gs, "gs" },
    { pitchclass::gss, "gss" },
    { pitchclass::aff, "aff" },
    { pitchclass::af, "af" },
    { pitchclass::a, "a" },
    { pitchclass::as, "as" },
    { pitchclass::ass, "ass" },
    { pitchclass::bff, "bff" },
    { pitchclass::bf, "bf" },
    { pitchclass::b, "b" },
    { pitchclass::bs, "bs" },
    { pitchclass::bss, "bss" }
};

std::string to_string(pitchclass p)
{
    auto it = pitchclass_names.find(p);
    if (it == pitchclass_names.end()) {
        throw exception(R"(cannot print invalid pitchclass {})",
                        static_cast<std::uint8_t>(p));
    }
    return it->second;
}

valid_pitchclass::valid_pitchclass()
{
    for (auto [key, value] : pitchclass_names) {
        emplace(key);
    }
}

staffline pitch::get_staffline() const
{
    // Compute the staff line offset, referenced to C4=0.

    static const std::map<pitchclass, std::uint8_t> line{
        { pitchclass::cff, 0 },
        { pitchclass::cf, 0 },
        { pitchclass::c, 0 },
        { pitchclass::cs, 0 },
        { pitchclass::css, 0 },
        { pitchclass::dff, 1 },
        { pitchclass::df, 1 },
        { pitchclass::d, 1 },
        { pitchclass::ds, 1 },
        { pitchclass::dss, 1 },
        { pitchclass::eff, 2 },
        { pitchclass::ef, 2 },
        { pitchclass::e, 2 },
        { pitchclass::es, 2 },
        { pitchclass::ess, 2 },
        { pitchclass::fff, 3 },
        { pitchclass::ff, 3 },
        { pitchclass::f, 3 },
        { pitchclass::fs, 3 },
        { pitchclass::fss, 3 },
        { pitchclass::gff, 4 },
        { pitchclass::gf, 4 },
        { pitchclass::g, 4 },
        { pitchclass::gs, 4 },
        { pitchclass::gss, 4 },
        { pitchclass::aff, 5 },
        { pitchclass::af, 5 },
        { pitchclass::a, 5 },
        { pitchclass::as, 5 },
        { pitchclass::ass, 5 },
        { pitchclass::bff, 6 },
        { pitchclass::bf, 6 },
        { pitchclass::b, 6 },
        { pitchclass::bs, 6 },
        { pitchclass::bss, 6 }
    };

    static const octave middle_C(4);
    return staffline(line.at(m_pitchclass) +
                     (static_cast<std::uint8_t>(m_octave - middle_C)) * 7);
};

bool operator<(const pitch &p1, const pitch &p2)
{
    if (p1.m_octave != p2.m_octave) {
        return p1.m_octave < p2.m_octave;
    }
    return p1.m_pitchclass < p2.m_pitchclass;
}

} // namespace stan
