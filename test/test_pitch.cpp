#include <mettle.hpp>

#include <stan/pitch.hpp>

#include <stan/rapidcheck/mettle.hpp>

mettle::property_suite<> value("pitch", [](auto &_) {
    using namespace mettle;
    using namespace stan;

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

    _.property("pitchclasses are valid", [](stan::pitchclass pc) {
        static const stan::valid_pitchclass valid;
        expect(valid, member(pc));
    });

    _.property("octaves are valid numbers", [](stan::octave oct) {
        expect(static_cast<std::uint8_t>(oct), all(greater_equal(0), less(8)));
    });

    _.test("to_string", []() {
        std::stringstream buffer;
        std::copy(all_pitchclasses.begin(), all_pitchclasses.end(),
                  std::ostream_iterator<pitchclass>(buffer, " "));
        expect(buffer.str(), equal_to("cff cf c cs css "
                                      "dff df d ds dss "
                                      "eff ef e es ess "
                                      "fff ff f fs fss "
                                      "gff gf g gs gss "
                                      "aff af a as ass "
                                      "bff bf b bs bss "));
    });

    _.test("sorting", []() {
        expect(pitch{ pc::a, octave(3) }, less(pitch{ pc::bf, octave(3) }));
        expect(pitch{ pc::a, octave(3) }, is_not(less(pitch{ pc::bf, octave(2) })));
    });
});
