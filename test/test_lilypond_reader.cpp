#include <stan/column.hpp>
#include <stan/driver/lilypond.hpp>
#include <stan/driver/debug.hpp>
#include "to_printable.hpp"
#include "property.hpp"

#include <mettle.hpp>

using mettle::equal_to;
using mettle::expect;
using mettle::thrown;

mettle::suite<
    stan::rest,
    stan::note,
    stan::chord,
    stan::beam,
    stan::tuplet,
    stan::meter,
    stan::clef
    >
    suite(
        "lilypond reader", mettle::type_only, [](auto &_) {
            static stan::lilypond::reader read;
            static stan::lilypond::writer write;
            static stan::driver::debug::writer debug;

            using Event = mettle::fixture_type_t<decltype(_)>;

            property(_, "writeread", [](Event n) {
                std::string lily = write(n);
                std::cout << lily << std::endl;
                expect(read(lily), equal_to<stan::column>(stan::column{ n }));
            });

            property(_, "parse error", [](Event n) {
                std::string lily = write(n) + " crash";
                expect([lily] { read(lily); },
                       thrown<std::runtime_error>("incomplete parse"));
            });
        });
