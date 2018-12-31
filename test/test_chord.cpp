#include <stan/notation.hpp>
#include "to_printable.hpp"
#include "property.hpp"

#include <mettle.hpp>

using mettle::equal_to;
using mettle::expect;
using mettle::thrown;

mettle::suite<> suite("chord", [](auto &_) {
    using namespace stan;
    using pc = stan::pitchclass;

    static const pitch c{ pc::c, octave{ 4 } };
    static const pitch e{ pc::e, octave{ 4 } };
    static const pitch f{ pc::f, octave{ 4 } };
    static const pitch a{ pc::a, octave{ 4 } };

    static const value quarter = value::quarter();
    static const value eighth = value::eighth();

    _.test("construction", []() {
        chord{ quarter, c, e };
        chord{ quarter, std::vector<pitch>{ c, e } };
    });

    _.test("invalid", []() {
        expect([] { chord{ quarter, std::vector<pitch>({ c }) }; },
               thrown<stan::invalid_chord>(
                   "invalid chord: at least two pitches required"));

        expect([] { chord{ quarter, std::vector<pitch>({ c, c }) }; },
               thrown<stan::invalid_chord>(
                   "invalid chord: unique pitches required"));

        expect([] { chord{ quarter, c }; },
               thrown<stan::invalid_chord>(
                   "invalid chord: at least two pitches required"));

        expect([] { chord{ quarter, c, c }; },
               thrown<stan::invalid_chord>(
                   "invalid chord: unique pitches required"));
    });
});
