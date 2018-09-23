#include <stan/column.hpp>
#include <stan/driver/lilypond.hpp>

#include <mettle.hpp>
#include <stan/rapidcheck/mettle.hpp>

using mettle::equal_to;
using mettle::expect;
using mettle::regex_match;

// We might like to use a fixture of stan::lilypond::writer, but this is blocked
// on proper Mettle support for properties.  See
// https://github.com/jimporter/mettle/issues/24.  So for now, we just
// instantiate the writer in each test.

mettle::property_suite<> lilypond_suite("lilypond writer", [](auto &_) {
    static stan::lilypond::writer write;
    _.test("octave", []() {
        using stan::octave;

        expect(write(octave{ 0 }), equal_to(",,,,"));
        expect(write(octave{ 1 }), equal_to(",,,"));
        expect(write(octave{ 2 }), equal_to(",,"));
        expect(write(octave{ 3 }), equal_to(","));
        expect(write(octave{ 4 }), equal_to(""));
        expect(write(octave{ 5 }), equal_to("'"));
        expect(write(octave{ 6 }), equal_to("''"));
        expect(write(octave{ 7 }), equal_to("'''"));
        expect(write(octave{ 8 }), equal_to("''''"));
    });

    _.test("pitchclass", []() {
        using pc = stan::pitchclass;

        expect(write(pc::cff), equal_to("cff"));
        expect(write(pc::cf), equal_to("cf"));
        expect(write(pc::c), equal_to("c"));
        expect(write(pc::cs), equal_to("cs"));
        expect(write(pc::css), equal_to("css"));
        expect(write(pc::dff), equal_to("dff"));
        expect(write(pc::df), equal_to("df"));
        expect(write(pc::d), equal_to("d"));
        expect(write(pc::ds), equal_to("ds"));
        expect(write(pc::dss), equal_to("dss"));
        expect(write(pc::eff), equal_to("eff"));
        expect(write(pc::ef), equal_to("ef"));
        expect(write(pc::e), equal_to("e"));
        expect(write(pc::es), equal_to("es"));
        expect(write(pc::ess), equal_to("ess"));
        expect(write(pc::fff), equal_to("fff"));
        expect(write(pc::ff), equal_to("ff"));
        expect(write(pc::f), equal_to("f"));
        expect(write(pc::fs), equal_to("fs"));
        expect(write(pc::fss), equal_to("fss"));
        expect(write(pc::gff), equal_to("gff"));
        expect(write(pc::gf), equal_to("gf"));
        expect(write(pc::g), equal_to("g"));
        expect(write(pc::gs), equal_to("gs"));
        expect(write(pc::gss), equal_to("gss"));
        expect(write(pc::aff), equal_to("aff"));
        expect(write(pc::af), equal_to("af"));
        expect(write(pc::a), equal_to("a"));
        expect(write(pc::as), equal_to("as"));
        expect(write(pc::ass), equal_to("ass"));
        expect(write(pc::bff), equal_to("bff"));
        expect(write(pc::bf), equal_to("bf"));
        expect(write(pc::b), equal_to("b"));
        expect(write(pc::bs), equal_to("bs"));
        expect(write(pc::bss), equal_to("bss"));
    });

    _.test("value", []() {
        using stan::value;

        expect(write(value::whole()), equal_to("1"));
        expect(write(dot(value::whole())), equal_to("1."));
        expect(write(dot(dot(value::whole()))), equal_to("1.."));
        expect(write(value::half()), equal_to("2"));
        expect(write(dot(value::half())), equal_to("2."));
        expect(write(dot(dot(value::half()))), equal_to("2.."));
        expect(write(value::quarter()), equal_to("4"));
        expect(write(dot(value::quarter())), equal_to("4."));
        expect(write(dot(dot(value::quarter()))), equal_to("4.."));
        expect(write(value::eighth()), equal_to("8"));
        expect(write(dot(value::eighth())), equal_to("8."));
        expect(write(dot(dot(value::eighth()))), equal_to("8.."));
        expect(write(value::sixteenth()), equal_to("16"));
        expect(write(dot(value::sixteenth())), equal_to("16."));
        expect(write(dot(dot(value::sixteenth()))), equal_to("16.."));
        expect(write(value::thirtysecond()), equal_to("32"));
        expect(write(dot(value::thirtysecond())), equal_to("32."));
        expect(write(value::sixtyfourth()), equal_to("64"));
        expect(write(value::instantaneous()), equal_to(""));
    });

    _.property("pitch", [](stan::pitch p) {
        expect(write(p), regex_match(write(p.m_pitchclass) + "[,']{0,4}"));
    });

    _.property("note", [](stan::note n) {
        // Values and pitches are arleady tested exhaustively, so here we check
        // that any possible note writes as a combination of those.
        // Correctness will be further checked in test_lilypond_reader, where
        // the test knows the correct answer.
        expect(write(n), equal_to(write(n.m_pitch) + write(n.m_value)));
    });
});
