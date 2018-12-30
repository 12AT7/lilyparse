#include <stan/column.hpp>
#include "to_printable.hpp"
#include "property.hpp"

#include <mettle.hpp>

using mettle::equal_to;
using mettle::expect;
using mettle::thrown;

mettle::suite<> suite("column", [](auto &_) {
    using namespace stan;

    property(_, "duration", [](column v) {
        expect(duration::zero() + v, mettle::greater_equal(duration::zero()));
    });
});
