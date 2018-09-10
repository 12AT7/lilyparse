#include <stan/column.hpp>
#include <stan/driver/lilypond.hpp>

#include <mettle.hpp>
#include <stan/rapidcheck/mettle.hpp>

using mettle::equal_to;
using mettle::expect;
using mettle::regex_match;

mettle::suite<> tuplet_suite("tuplet", [](auto &_) {
        using stan::tuplet;
        using stan::pitch;
        using stan::rest;
        using stan::chord;
        using stan::octave;
        using stan::value;
        using stan::note;
        using pc = stan::pitchclass;

        static const pitch c { pc::c, octave{4} };
        static const pitch e { pc::e, octave{4} };
        static const pitch f { pc::f, octave{4} };
        static const pitch a { pc::a, octave{4} };
        static const value quarter = value::quarter();
        static const value eighth = value::eighth();
        static const note c8 { eighth, { c } };

        _.test("construction", []() {
#if 0
                tuplet{quarter, { c8, c8, c8 }};
                tuplet{quarter, {
                        chord{eighth, {c, e}},
                        chord{eighth, {f, a}},
                        chord{eighth, {c, e}}
                        }};
                tuplet{quarter, {
                        chord{quarter, {c, e}},
                        chord{eighth, {f, a}}
                        }};
                tuplet{quarter, {
                        chord{eighth, {c, e}},
                        rest{eighth},
                        note{eighth, c}
                        }};
                tuplet{quarter, {
                        tuplet{eighth, { c8, c8, c8 }},
                        tuplet{eighth, { c8, c8, c8 }}
                    }};
#endif
                });
        });

