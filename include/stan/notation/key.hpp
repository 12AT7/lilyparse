#pragma once

#include <stan/notation/pitch.hpp>
#include <stan/exception.hpp>

#include <boost/hana/define_struct.hpp>

#include <vector>
#include <numeric>
#include <iostream>

namespace stan {
  
namespace mode {

static const std::vector<std::uint8_t> major { 0, 2, 4, 5, 7, 9, 11 };
static const std::vector<std::uint8_t> minor { 0, 2, 3, 5, 7, 8, 10 };

}

struct invalid_key : exception
{
    template <typename... Args>
    invalid_key(const char *format, Args... args) :
        exception((std::string("invalid key: ") + format).c_str(),
                  std::forward<Args>(args)...) {}
};

struct key {

    BOOST_HANA_DEFINE_STRUCT(key,
            (pitchclass, m_tonic),
            (std::vector<std::uint8_t>, m_mode)
    );

    // Key construction is rare, but every note has to be checked
    // against the key to get the accidentals right every time music is
    // rendered.  So make the containment check as fast as possible. This table
    // is just a different normalization of m_tonic and m_mode and is computed
    // from those, used to accelerate the frequent containment check.
    std::array<bool, 256> m_fastcheck { false };

    bool contains(pitchclass pc) const { 
        return m_fastcheck[static_cast<std::uint8_t>(pc)]; 
    }

    bool contains(pitch p) const { 
        return m_fastcheck[static_cast<std::uint8_t>(p.m_pitchclass)]; 
    }

    key(pitchclass tonic, std::vector<std::uint8_t> mode) 
	    : m_tonic(tonic), m_mode(mode)
    {
	if (mode.size() != 7)
	{
	    // Major and minor are probably the only modes ever explicitly
	    // indicated by a key signature.  Semantics of a "key" object for
	    // non 7 note modes (like whole tone or diminished scales) are
	    // really not clear at all, and probably never needed either.  We
	    // just do not have standard notations for key-like entities for
	    // anything other than minor or major.
    	    throw invalid_key("only standard 7 pitch modes are supported");
	}

	for (std::uint16_t degree = 0; degree < mode.size(); ++degree)
        {
            std::int16_t pitchcode = 
                static_cast<std::uint16_t>(tonic) // start with the tonic 
                    + 0x10*degree // add the scale degree
                    + mode[degree] - 2*degree // add the mode's accidental
                    ;

            // Deal with wrap around from the b range back to c.  The 0x70 term
            // aliases big numbers back to c, and the -2 term accounts for the
            // scale being 12 pitches and not 7*2 = 14, because of the half
            // steps between e->f and b->c.
            if (pitchcode > static_cast<std::uint8_t>(pitchclass::bss))
	    {
                pitchcode -= 0x70 - 2;
	    }

	    m_fastcheck[static_cast<std::uint8_t>(pitchcode)] = true;
        }
    }

    std::vector<pitchclass> scale() const
    {
	static const valid_pitchclass pitches;

	std::vector<pitchclass> s;
	std::uint8_t tonic = static_cast<std::uint8_t>(m_tonic);
	for (std::uint8_t i = tonic; i+1 != tonic; ++i)
	{
	    pitchclass pc { i };
	    if (contains(pc))
		s.push_back(pc);
	}
	return s;
    }
};

} // namespace stan
