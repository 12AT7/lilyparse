#include <stan/notation.hpp>
#include <stan/driver/debug.hpp>

#include <mettle.hpp>
#include "property.hpp"

using mettle::equal_to;
using mettle::expect;
using mettle::member;

mettle::suite<> pitchclass_suite("pitchclass", [](auto &_) {
    using pc = stan::pitchclass;

    static const std::vector<stan::pitchclass> all_pitchclasses{
        pc::cff, pc::cf, pc::c, pc::cs, pc::css,
        pc::dff, pc::df, pc::d, pc::ds, pc::dss,
        pc::eff, pc::ef, pc::e, pc::es, pc::ess,
        pc::fff, pc::ff, pc::f, pc::fs, pc::fss,
        pc::gff, pc::gf, pc::g, pc::gs, pc::gss,
        pc::aff, pc::af, pc::a, pc::as, pc::ass,
        pc::bff, pc::bf, pc::b, pc::bs, pc::bss
    };

    property(_, "pitchclasses are valid", [](stan::pitchclass pc) {
        static const stan::valid_pitchclass valid;
        expect(valid, member(pc));
    });

    // _.test("to_string", []() {
    //     std::stringstream buffer;
    //     stan::string_generator<std::vector<stan::pitchclass>> generate;
    //     expect(generate(all_pitchclasses),
    //            equal_to("cff cf c cs css "
    //                     "dff df d ds dss "
    //                     "eff ef e es ess "
    //                     "fff ff f fs fss "
    //                     "gff gf g gs gss "
    //                     "aff af a as ass "
    //                     "bff bf b bs bss"));
    // });
});

mettle::suite<> pitch_suite("pitch", [](auto &_) {
    using mettle::less;
    using mettle::is_not;
    using mettle::greater_equal;

    using stan::pitch;
    using stan::octave;
    using pc = stan::pitchclass;

    property(_, "octaves are valid numbers", [](stan::octave oct) {
        expect(static_cast<std::uint8_t>(oct), all(greater_equal(0), less(8)));
    });

    property(_, "pitches", [](stan::pitch p) {
        expect(static_cast<std::uint8_t>(p.get_staffline()), all(greater_equal(0)));
    });

    _.test("sorting", []() {
        expect(pitch{ pc::a, octave(3) }, less(pitch{ pc::bf, octave(3) }));
        expect(pitch{ pc::a, octave(3) }, is_not(less(pitch{ pc::bf, octave(2) })));
    });
});
