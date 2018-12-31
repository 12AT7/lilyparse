#include <stan/notation.hpp>
#include "property.hpp"
#include "to_printable.hpp"

#include <mettle.hpp>

using mettle::equal_to;
using mettle::expect;
using mettle::thrown;

mettle::suite<> suite("key", [](auto &_) {
    using namespace stan;
    using pc = stan::pitchclass;

    const std::vector<pc> A_minor { pc::a, pc::b, pc::c, pc::d, pc::e, pc::f, pc::g };
    const std::vector<pc> Bf_minor { pc::bf, pc::c, pc::df, pc::ef, pc::f, pc::gf, pc::af };
    const std::vector<pc> B_minor { pc::b, pc::cs, pc::d, pc::e, pc::fs, pc::g, pc::a };
    const std::vector<pc> C_minor { pc::c, pc::d, pc::ef, pc::f, pc::g, pc::af, pc::bf };
    const std::vector<pc> Df_minor { pc::df, pc::ef, pc::ff, pc::gf, pc::af, pc::bff, pc::cf };
    const std::vector<pc> D_minor { pc::d, pc::e, pc::f, pc::g, pc::a, pc::bf, pc::c };
    const std::vector<pc> Ef_minor { pc::ef, pc::f, pc::gf, pc::af, pc::bf, pc::cf, pc::df };
    const std::vector<pc> E_minor { pc::e, pc::fs, pc::g, pc::a, pc::b, pc::c, pc::d };
    const std::vector<pc> F_minor { pc::f, pc::g, pc::af, pc::bf, pc::c, pc::df, pc::ef };
    const std::vector<pc> Gf_minor { pc::gf, pc::af, pc::bff, pc::cf, pc::df, pc::eff, pc::ff };
    const std::vector<pc> G_minor { pc::g, pc::a, pc::bf, pc::c, pc::d, pc::ef, pc::f };
    const std::vector<pc> Af_minor { pc::af, pc::bf, pc::cf, pc::df, pc::ef, pc::ff, pc::gf };

    const std::vector<pc> A_major { pc::a, pc::b, pc::cs, pc::d, pc::e, pc::fs, pc::gs };
    const std::vector<pc> Bf_major { pc::bf, pc::c, pc::d, pc::ef, pc::f, pc::g, pc::a };
    const std::vector<pc> B_major { pc::b, pc::cs, pc::ds, pc::e, pc::fs, pc::gs, pc::as };
    const std::vector<pc> C_major { pc::c, pc::d, pc::e, pc::f, pc::g, pc::a, pc::b };
    const std::vector<pc> Df_major { pc::df, pc::ef, pc::f, pc::gf, pc::af, pc::bf, pc::c };
    const std::vector<pc> D_major { pc::d, pc::e, pc::fs, pc::g, pc::a, pc::b, pc::cs };
    const std::vector<pc> Ef_major { pc::ef, pc::f, pc::g, pc::af, pc::bf, pc::c, pc::d };
    const std::vector<pc> E_major { pc::e, pc::fs, pc::gs, pc::a, pc::b, pc::cs, pc::ds };
    const std::vector<pc> F_major { pc::f, pc::g, pc::a, pc::bf, pc::c, pc::d, pc::e };
    const std::vector<pc> Gf_major { pc::gf, pc::af, pc::bf, pc::cf, pc::df, pc::ef, pc::f };
    const std::vector<pc> G_major { pc::g, pc::a, pc::b, pc::c, pc::d, pc::e, pc::fs };
    const std::vector<pc> Af_major { pc::af, pc::bf, pc::c, pc::df, pc::ef, pc::f, pc::g };

    _.test("construction", [=]() {
            expect(key(pc::a, mode::major).scale(), equal_to(A_major));
            expect(key(pc::bf, mode::major).scale(), equal_to(Bf_major));
            expect(key(pc::b, mode::major).scale(), equal_to(B_major));
            expect(key(pc::c, mode::major).scale(), equal_to(C_major));
            expect(key(pc::df, mode::major).scale(), equal_to(Df_major));
            expect(key(pc::d, mode::major).scale(), equal_to(D_major));
            expect(key(pc::ef, mode::major).scale(), equal_to(Ef_major));
            expect(key(pc::e, mode::major).scale(), equal_to(E_major));
            expect(key(pc::f, mode::major).scale(), equal_to(F_major));
            expect(key(pc::gf, mode::major).scale(), equal_to(Gf_major));
            expect(key(pc::g, mode::major).scale(), equal_to(G_major)); 
            expect(key(pc::af, mode::major).scale(), equal_to(Af_major));
            expect(key(pc::a, mode::minor).scale(), equal_to(A_minor));
            expect(key(pc::bf, mode::minor).scale(), equal_to(Bf_minor));
            expect(key(pc::b, mode::minor).scale(), equal_to(B_minor));
            expect(key(pc::c, mode::minor).scale(), equal_to(C_minor));
            expect(key(pc::df, mode::minor).scale(), equal_to(Df_minor));
            expect(key(pc::d, mode::minor).scale(), equal_to(D_minor));
            expect(key(pc::ef, mode::minor).scale(), equal_to(Ef_minor));
            expect(key(pc::e, mode::minor).scale(), equal_to(E_minor));
            expect(key(pc::f, mode::minor).scale(), equal_to(F_minor));
            expect(key(pc::gf, mode::minor).scale(), equal_to(Gf_minor));
            expect(key(pc::g, mode::minor).scale(), equal_to(G_minor)); 
            expect(key(pc::af, mode::minor).scale(), equal_to(Af_minor));
            });

    _.test("containment", [=]() {
            key A { pc::a, mode::minor };
            using std::placeholders::_1;
            expect(A_minor, each(filter([&A](pc p) { return A.contains(p); }, equal_to(true)))); 
        });

    property(_, "operator==", [](const key& k1) {
            key k2 = k1;
            expect(k1, mettle::equal_to(k2));
        }); 
});
