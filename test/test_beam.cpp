#include <stan/notation.hpp>
#include <stan/driver/lilypond.hpp>
#include "to_printable.hpp"
#include "property.hpp"

#include <mettle.hpp>

using mettle::equal_to;
using mettle::expect;
using mettle::regex_match;
using mettle::thrown;

mettle::suite<> suite("beam", [](auto &_) {
    using namespace stan;
    using pc = stan::pitchclass;

    static const pitch c{ pc::c, octave{ 4 } };
    static const pitch e{ pc::e, octave{ 4 } };
    static const pitch f{ pc::f, octave{ 4 } };
    static const pitch a{ pc::a, octave{ 4 } };
    static const value quarter = value::quarter();
    static const value eighth = value::eighth();
    static const note c8{ eighth, { c } };
    static const note c4{ quarter, { c } };

    _.test("construction", []() {
        beam{ c8, c8 };
        beam{ chord{ value::eighth(), c, e }, chord{ value::eighth(), c, e } };
        beam{ tuplet{ value::quarter(), c8, c8, c8 } };

        // Explicitly instantiate variant to avoid confusion with copy constructor
        beam{ column(beam{ c8, c8 }), column(beam{ c8, c8 }) };
    });

    _.test("invalid", []() {
        expect([] { beam{ rest{ quarter }, c8 }; },
               thrown<stan::invalid_beam>("invalid beam: cannot contain rests"));

        expect([] { beam{ note{ value::whole(), c }, note{ value::whole(), c } }; },
               thrown<stan::invalid_beam>(
                   "invalid beam: cannot contain whole or half notes"));

        expect([] { beam{ c8 }; },
               thrown<stan::invalid_beam>(
                   "invalid beam: must contain at least two elements"));

        expect([] { beam{ chord{ value::whole(), c, e },
                          chord{ value::whole(), c, e } }; },
               thrown<stan::invalid_beam>(
                   "invalid beam: cannot contain whole or half notes"));

        expect([] { beam{ chord{ value::quarter(), c, e } }; },
               thrown<stan::invalid_beam>(
                   "invalid beam: must contain at least two elements"));

        expect([] { beam{ tuplet{ value::half(), c4, c4, c4 } }; },
               thrown<stan::invalid_beam>(
                   "invalid beam: cannot contain whole or half note tuplets"));

        expect([] { beam{ column(beam{ c8, c8 }) }; },
               thrown<stan::invalid_beam>(
                   "invalid beam: nested beams must contain at least two elements"));
    });
});
