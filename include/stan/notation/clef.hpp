#pragma once

#include <boost/hana/define_struct.hpp>

namespace stan {

struct clef
{
	enum struct type : std::uint8_t
	{
		treble, alto, tenor, bass, percussion
	};

    BOOST_HANA_DEFINE_STRUCT(clef,
                             (type, m_type)
			     // (bool, m_change));
                             // (char32_t, glyph),
                             // (std::int8_t, offset),
                             // (std::int8_t, transpose)
			     );

    clef(type t) : m_type(t) {}
    // constexpr clef() :
    //     clef(clef::treble()) {}

    // constexpr clef(const char *name, char32_t glyph, std::int8_t offset, std::int8_t transpose) :
    //     name(name), glyph(glyph), offset(offset), transpose(transpose) {}

    // static constexpr clef bass()
    // {
    //     return clef{ "bass", glyphs::clefs::F, 7, -12 };
    // }
    // static constexpr clef bass_change()
    // {
    //     return clef{ "bass", glyphs::clefs::F_change, 7, -12 };
    // }
    // static constexpr clef treble()
    // {
    //     return clef{ "treble", glyphs::clefs::G, 5, 0 };
    // }
    // static constexpr clef treble_change()
    // {
    //     return clef{ "treble", glyphs::clefs::G_change, 5, 0 };
    // }
    // static constexpr clef alto()
    // {
    //     return clef{ "alto", glyphs::clefs::C, 0, 0 };
    // }
    // static constexpr clef alto_change()
    // {
    //     return clef{ "alto", glyphs::clefs::C_change, 0, 0 };
    // }
    // static constexpr clef tenor()
    // {
    //     return clef{ "tenor", glyphs::clefs::C, 3, 0 };
    // }
    // static constexpr clef tenor_change()
    // {
    //     return clef{ "tenor", glyphs::clefs::C_change, 3, 0 };
    // }
    // static constexpr clef percussion()
    // {
    //     return clef{ "percussion", glyphs::clefs::percussion, 0, 0 };
    // }
    // static constexpr clef percussion_change()
    // {
    //     return clef{ "percussion", glyphs::clefs::percussion_change, 0, 0 };
    // }

    // friend bool operator==(const clef &t1, const clef &t2)
    // {
    //     return t1.glyph == t2.glyph;
    // }

    // friend bool operator!=(const clef &t1, const clef &t2)
    // {
    //     return t1.glyph != t2.glyph;
    // }

    // friend bool operator<(const clef &t1, const clef &t2)
    // {
    //     return t1.glyph < t2.glyph;
    // }
};

} // namespace stan
