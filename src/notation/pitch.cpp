#include <lilyparse/exception.hpp>
#include <lilyparse/pitch.hpp>

#include <algorithm>
#include <map>

namespace lilyparse {

using pc = pitchclass;
static const std::map<pitchclass, const char *> pitchclass_names = {
    { pc::cff, "cff" }, { pc::cf, "cf" }, { pc::c, "c" }, { pc::cs, "cs" },
    { pc::css, "css" }, { pc::dff, "dff" }, { pc::df, "df" }, { pc::d, "d" },
    { pc::ds, "ds" }, { pc::dss, "dss" }, { pc::eff, "eff" }, { pc::ef, "ef" },
    { pc::e, "e" }, { pc::es, "es" }, { pc::ess, "ess" }, { pc::fff, "fff" },
    { pc::ff, "ff" }, { pc::f, "f" }, { pc::fs, "fs" }, { pc::fss, "fss" },
    { pc::gff, "gff" }, { pc::gf, "gf" }, { pc::g, "g" }, { pc::gs, "gs" },
    { pc::gss, "gss" }, { pc::aff, "aff" }, { pc::af, "af" }, { pc::a, "a" },
    { pc::as, "as" }, { pc::ass, "ass" }, { pc::bff, "bff" }, { pc::bf, "bf" },
    { pc::b, "b" }, { pc::bs, "bs" }, { pc::bss, "bss" }
};

std::string to_string(pitchclass p)
{
    auto it = pitchclass_names.find(p);
    if (it == pitchclass_names.end())
        throw exception(R"(cannot print invalid pitchclass {})",
            static_cast<std::uint8_t>(p));
    return it->second;
}

std::string to_string(const pitch &p)
{
    std::string lily = to_string(p.pitchclass_);
    std::uint8_t rawoctave = static_cast<std::uint8_t>(p.octave_);
    std::copy_n(",,,,", 4 - rawoctave, std::back_inserter(lily));
    std::copy_n("''''", rawoctave - 4, std::back_inserter(lily));
    return lily;
}

valid_pitchclass::valid_pitchclass()
{
    for (auto [key, value] : pitchclass_names)
        emplace(key);
}

midi::pitch pitch::get_midi() const
{
    // Compute the actual pitch number, using the MIDI convention C4=60.
    // As this reduction discards all subtlety about enharmonics, it is
    // used only in specific places that need this representation (like
    // MIDI note_on/note_off and geometry calculations).  There is no way
    // to go backwards, from int to pitch, because of the enharmonic
    // ambiguity.

    using pc = lilyparse::pitchclass;
    static const std::map<pitchclass, std::uint8_t> midi{ { pc::cff, 58 },
        { pc::cf, 59 }, { pc::c, 60 }, { pc::cs, 61 }, { pc::css, 62 },
        { pc::dff, 60 }, { pc::df, 61 }, { pc::d, 62 }, { pc::ds, 63 },
        { pc::dss, 64 }, { pc::eff, 62 }, { pc::ef, 63 }, { pc::e, 64 },
        { pc::es, 65 }, { pc::ess, 66 }, { pc::fff, 63 }, { pc::ff, 64 },
        { pc::f, 65 }, { pc::fs, 66 }, { pc::fss, 67 }, { pc::gff, 65 },
        { pc::gf, 66 }, { pc::g, 67 }, { pc::gs, 68 }, { pc::gss, 69 },
        { pc::aff, 67 }, { pc::af, 68 }, { pc::a, 69 }, { pc::as, 70 },
        { pc::ass, 71 }, { pc::bff, 69 }, { pc::bf, 70 }, { pc::b, 71 },
        { pc::bs, 72 }, { pc::bss, 73 } };

    return midi::pitch(
        midi.at(pitchclass_) + (static_cast<std::uint8_t>(octave_) - 4) * 12);
}

staffline pitch::get_staffline() const
{
    // Compute the staff line offset, referenced to C4=0.

    using pc = lilyparse::pitchclass;
    static const std::map<pitchclass, std::uint8_t> line{ { pc::cff, 0 },
        { pc::cf, 0 }, { pc::c, 0 }, { pc::cs, 0 }, { pc::css, 0 },
        { pc::dff, 1 }, { pc::df, 1 }, { pc::d, 1 }, { pc::ds, 1 },
        { pc::dss, 1 }, { pc::eff, 2 }, { pc::ef, 2 }, { pc::e, 2 },
        { pc::es, 2 }, { pc::ess, 2 }, { pc::fff, 3 }, { pc::ff, 3 },
        { pc::f, 3 }, { pc::fs, 3 }, { pc::fss, 3 }, { pc::gff, 4 },
        { pc::gf, 4 }, { pc::g, 4 }, { pc::gs, 4 }, { pc::gss, 4 },
        { pc::aff, 5 }, { pc::af, 5 }, { pc::a, 5 }, { pc::as, 5 },
        { pc::ass, 5 }, { pc::bff, 6 }, { pc::bf, 6 }, { pc::b, 6 },
        { pc::bs, 6 }, { pc::bss, 6 } };

    static const octave middle_C(4);
    return staffline(line.at(pitchclass_) +
        (static_cast<std::uint8_t>(octave_ - middle_C)) * 7);
};

std::ostream &operator<<(std::ostream &os, const pitch &p)
{
    return os << to_string(p);
}

bool operator<(const pitch &p1, const pitch &p2)
{
    if (p1.octave_ != p2.octave_)
        return p1.octave_ < p2.octave_;
    return p1.pitchclass_ < p2.pitchclass_;
}

std::ostream &operator<<(std::ostream &os, const pitchclass &pc)
{
    return os << to_string(pc);
}

} // namespace lilyparse
