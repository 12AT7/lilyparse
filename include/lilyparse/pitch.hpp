#pragma once

#include <type_safe/strong_typedef.hpp>
#include <boost/hana/define_struct.hpp>
#include <set>

namespace lilyparse {

namespace ts = type_safe;

// Pitch models "American Standard Pitch Notation" -
// https://en.wikipedia.org/wiki/Scientific_pitch_notation

// Whoever designed the system of Western harmony was definitely not a software
// engineer.  Enharmonic pitches and oddball half steps between b->c and e->f
// make for bug prone computer models of harmony.  Thanks, Bach!
enum struct pitchclass : std::uint8_t 
{ 
    // These magic offsets work in concert with the lilyparse::key()
    // constructor that has to compute any mode from any tonic.
    cff = 0x02, cf, c, cs, css,
    dff = 0x12, df, d, ds, dss,
    eff = 0x22, ef, e, es, ess,
    fff = 0x31, ff, f, fs, fss,
    gff = 0x41, gf, g, gs, gss,
    aff = 0x51, af, a, as, ass,
    bff = 0x61, bf, b, bs, bss,
};

// An octave is a std::uint8_t, with very limited semantics
struct octave : ts::strong_typedef<octave, std::uint8_t>, 
	ts::strong_typedef_op::addition<octave>,
	ts::strong_typedef_op::subtraction<octave>,
	ts::strong_typedef_op::equality_comparison<octave>,
	ts::strong_typedef_op::relational_comparison<octave>
{
    using ts::strong_typedef<octave, std::uint8_t>::strong_typedef;
};

struct staffline : ts::strong_typedef<staffline, std::uint8_t> 
{
    using ts::strong_typedef<staffline, std::uint8_t>::strong_typedef;
};

namespace midi {
    struct pitch : ts::strong_typedef<pitch, std::uint8_t> 
    {
    	using ts::strong_typedef<pitch, std::uint8_t>::strong_typedef;
    };
}

struct pitch 
{
    BOOST_HANA_DEFINE_STRUCT(pitch,
        (lilyparse::pitchclass, pitchclass_),
        (lilyparse::octave, octave_)
    );

    midi::pitch get_midi() const;
    staffline get_staffline() const; 

    friend std::string to_string(const pitch& p);
    friend std::ostream& operator<<(std::ostream&, const pitch&);

    friend bool operator<(const pitch&, const pitch&);
    friend bool operator==(const pitch&, const pitch&);
};

extern std::string to_string(pitchclass);
extern std::ostream& operator<<(std::ostream&, const pitchclass&);

// Enumerating all of valid pitchclasses is useful, especially in testing, but
// their weird numbering scheme inhibits an easy iteration.  C++ also lacks the
// introspection needed to directly iterate over an enumeration class.  So,
// here the set of valid pitchlasses is built by iterating the whole range of
// std::uint8_t, and noticing if to_string(pitchclass) can determine a value.
// It's a hack, but it saves a lot of trouble in rapidcheck/music.hpp and
// elsewhere.
struct valid_pitchclass : std::set<pitchclass> 
{
    valid_pitchclass();
};


} // music
