#include <stan/column.hpp>
#include <stan/duration.hpp>
#include <stan/driver/lilypond.hpp>
#include "to_printable.hpp"

#include <mettle.hpp>
#include <stan/rapidcheck/mettle.hpp>

using mettle::equal_to;
using mettle::expect;
using mettle::regex_match;

mettle::suite<> beam_suite("tuplet", [](auto &_) {
    using stan::pitch;
    using stan::rest;
    using stan::chord;
    using stan::octave;
    using stan::value;
    using stan::note;
    using stan::beam;
    using stan::tuplet;
    using pc = stan::pitchclass;

    static const pitch c{ pc::c, octave{ 4 } };
    static const pitch e{ pc::e, octave{ 4 } };
    static const pitch f{ pc::f, octave{ 4 } };
    static const pitch a{ pc::a, octave{ 4 } };
    static const value quarter = value::quarter();
    static const value eighth = value::eighth();
    static const note c8{ eighth, { c } };

    _.test("construction", []() {
        beam{ c8, c8 };
        beam{ chord{ value::eighth(), c, e }, chord{ value::eighth(), c, e } };
        beam{ tuplet{ value::quarter(), c8, c8, c8 } };
        beam{ beam{ c8, c8 }, beam{ c8, c8 } };
    });
});
