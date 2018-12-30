#pragma once

namespace stan {

struct clef
{

    BOOST_HANA_DEFINE_STRUCT(clef,
                             (const char *, name),
                             (char32_t, glyph),
                             (std::int8_t, offset),
                             (std::int8_t, transpose));

    constexpr clef() :
        clef(clef::treble()) {}

    constexpr clef(const char *name, char32_t glyph, std::int8_t offset, std::int8_t transpose) :
        name(name), glyph(glyph), offset(offset), transpose(transpose) {}

    static constexpr clef bass()
    {
        return clef{ "bass", glyphs::clefs::F, 7, -12 };
    }
    static constexpr clef bass_change()
    {
        return clef{ "bass", glyphs::clefs::F_change, 7, -12 };
    }
    static constexpr clef treble()
    {
        return clef{ "treble", glyphs::clefs::G, 5, 0 };
    }
    static constexpr clef treble_change()
    {
        return clef{ "treble", glyphs::clefs::G_change, 5, 0 };
    }
    static constexpr clef alto()
    {
        return clef{ "alto", glyphs::clefs::C, 0, 0 };
    }
    static constexpr clef alto_change()
    {
        return clef{ "alto", glyphs::clefs::C_change, 0, 0 };
    }
    static constexpr clef tenor()
    {
        return clef{ "tenor", glyphs::clefs::C, 3, 0 };
    }
    static constexpr clef tenor_change()
    {
        return clef{ "tenor", glyphs::clefs::C_change, 3, 0 };
    }
    static constexpr clef percussion()
    {
        return clef{ "percussion", glyphs::clefs::percussion, 0, 0 };
    }
    static constexpr clef percussion_change()
    {
        return clef{ "percussion", glyphs::clefs::percussion_change, 0, 0 };
    }

    friend bool operator==(const clef &t1, const clef &t2)
    {
        return t1.glyph == t2.glyph;
    }

    friend bool operator!=(const clef &t1, const clef &t2)
    {
        return t1.glyph != t2.glyph;
    }

    friend bool operator<(const clef &t1, const clef &t2)
    {
        return t1.glyph < t2.glyph;
    }

    friend std::string to_string(const clef &t)
    {
        return std::string("\\clef ") + t.name;
    }

    friend std::ostream &operator<<(std::ostream &os, const clef &t)
    {
        return os << to_string(t);
    }
};

} // namespace stan
