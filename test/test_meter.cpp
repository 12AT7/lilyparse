#include <stan/notation.hpp>
#include "to_printable.hpp"
#include "property.hpp"

#include <mettle.hpp>

using mettle::equal_to;
using mettle::expect;
using mettle::regex_match;
using mettle::thrown;

stan::driver::debug::writer debug;

mettle::suite<> meter_suite("meter", [](auto &_) {
    using namespace stan;

    _.test("construction", []() {
        meter{ { 4 }, value::quarter() };
    });

    _.test("invalid", []() {
        expect([] { meter{ {}, value::quarter() }; },
               thrown<invalid_meter>("invalid meter: no beats"));
        expect([] { meter{ { 3 }, dot(value::quarter()) }; },
               thrown<invalid_meter>(
                   "invalid meter: value must be half, quarter, "
                   "eighth, sixteenth, or thirtysecond"));
    });

    property(_, "property", [](meter m) {
    });
});
