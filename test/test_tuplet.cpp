#include <stan/notation.hpp>
#include <stan/driver/lilypond.hpp>

#include "to_printable.hpp"
#include "property.hpp"

#include <mettle.hpp>

using mettle::equal_to;
using mettle::expect;
using mettle::regex_match;
using mettle::thrown;

mettle::suite<> tuplet_suite("tuplet", [](auto &_) {
    using namespace stan;
    using pc = stan::pitchclass;

    static const pitch c{ pc::c, octave{ 4 } };
    static const pitch e{ pc::e, octave{ 4 } };
    static const pitch f{ pc::f, octave{ 4 } };
    static const pitch a{ pc::a, octave{ 4 } };
    static const value quarter = value::quarter();
    static const value eighth = value::eighth();
    static const note c8{ eighth, { c } };

    static stan::driver::debug::writer debug;

    // clang-format off
        _.test("construction", []() {
                tuplet{quarter, c8, c8, c8 };
                tuplet{quarter, 
                        chord{eighth, c, e},
                        chord{eighth, f, a},
                        chord{eighth, c, e}
                        };
                tuplet{quarter, 
                        chord{quarter, c, e},
                        chord{eighth, f, a}
                        };
                tuplet{quarter, 
                        chord{eighth, c, e},
                        rest{eighth},
                        note{eighth, c}
                        };
                tuplet{quarter, 
                        tuplet{eighth, c8, c8, c8 },
                        tuplet{eighth, c8, c8, c8 }
                    };
        // clang-format on
    });

    _.test("invalid", []() {
        expect([] { tuplet{ quarter, c8 }; },
               thrown<stan::invalid_tuplet>(
                   "invalid tuplet: must contain at least two elements"));
    });

    using rational = stan::rational<std::uint16_t>;
    property(_, "compute rational", [](rational r) {
        float real = r; // Invoke rational::operator float()
        expect(rational::quantize(real), equal_to(r));
    });

    _.test("scaling", []() {
        expect(tuplet::scale(1, 1, value::quarter()), equal_to(value::quarter()));

        // Triplets
        expect(tuplet::scale(3, 2, dot(value::thirtysecond())), equal_to(value::thirtysecond()));
        expect(tuplet::scale(3, 2, dot(value::sixteenth())), equal_to(value::sixteenth()));
        expect(tuplet::scale(3, 2, dot(value::eighth())), equal_to(value::eighth()));
        expect(tuplet::scale(3, 2, dot(value::quarter())), equal_to(value::quarter()));
        expect(tuplet::scale(3, 2, dot(value::half())), equal_to(value::half()));
        expect(tuplet::scale(3, 2, dot(value::whole())), equal_to(value::whole()));

        // Two in Three
        expect(tuplet::scale(2, 3, value::quarter()), equal_to(dot(value::quarter())));

        // Quintuplets
        expect(tuplet::scale(5, 4, 5 * value::thirtysecond()), equal_to(value::eighth()));
        expect(tuplet::scale(5, 4, 5 * value::sixteenth()), equal_to(value::quarter()));
        expect(tuplet::scale(5, 4, 5 * value::eighth()), equal_to(value::half()));
        expect(tuplet::scale(5, 4, 5 * value::quarter()), equal_to(value::whole()));

        // Sextuplets
        expect(tuplet::scale(6, 4, 6 * value::thirtysecond()), equal_to(value::eighth()));
        expect(tuplet::scale(6, 4, 6 * value::sixteenth()), equal_to(value::quarter()));
        expect(tuplet::scale(6, 4, 6 * value::eighth()), equal_to(value::half()));
        expect(tuplet::scale(6, 4, 6 * value::quarter()), equal_to(value::whole()));

        // Septuplets
        expect(tuplet::scale(7, 4, 7 * value::thirtysecond()), equal_to(value::eighth()));
        expect(tuplet::scale(7, 4, 7 * value::sixteenth()), equal_to(value::quarter()));
        expect(tuplet::scale(7, 4, 7 * value::eighth()), equal_to(value::half()));
        expect(tuplet::scale(7, 4, 7 * value::quarter()), equal_to(value::whole()));
    });

    property(_, "generate", [](tuplet v) {
        stan::duration d = v; // Invoke cast operator
        expect(d, mettle::greater(duration::zero()));
    });

    _.test("invalid", []() {
        expect([] { tuplet::scale(3, 7, value::quarter()); },
               thrown<stan::invalid_tuplet>());
    });
});
