#include <stan/column.hpp>
#include <stan/driver/lilypond.hpp>
#include <stan/driver/debug.hpp>
#include "to_printable.hpp"

#include <mettle.hpp>
#include <stan/rapidcheck/mettle.hpp>

using mettle::equal_to;
using mettle::expect;
using mettle::regex_match;

mettle::property_suite<> lilypond_suite("lilypond reader", [](auto &_) {
    using pc = stan::pitchclass;
    using stan::value;
    using stan::octave;
    using stan::pitch;

    _.test("note", []() {
        stan::lilypond::reader read;
        using stan::note;

        note truth{ value::quarter(), pitch{ pc::c, octave{ 4 } } };
        expect(read("c4"), equal_to(truth));
    });
});
