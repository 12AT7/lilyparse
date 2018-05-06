#include <mettle.hpp>
#include <lilyparse/rapidcheck/mettle.hpp>

#include <lilyparse/value.hpp>

using namespace lilyparse;

mettle::property_suite<> suite("value", [](auto &_) {
    using namespace mettle;

    _.test("print valid values", []() {
        std::stringstream buffer;
        std::copy(all_values.begin(), all_values.end(),
            std::ostream_iterator<value>(buffer, " "));
        expect(buffer.str(), equal_to("1 2 4 8 16 32 64 "
                                      "1. 2. 4. 8. 16. 32. "
                                      "1.. 2.. 4.. 8.. 16.. "));
    });

    _.test("duration", []() {
        // durations should be the equivalent number of 64th notes
        std::vector<duration> truth{
            duration(64), duration(32), duration(16), duration(8), duration(4), duration(2), duration(1), duration(96), duration(48), duration(24), duration(12), duration(6), duration(3), duration(112), duration(56), duration(28), duration(14), duration(7)
        };
        // transform all_values to the ints.  Could not get mettle::array
        // matcher to work.
        std::vector<duration> durations(all_values.size());
        std::transform(all_values.begin(), all_values.end(), durations.begin(),
            [](value v) { return duration(v); });
        expect(durations, equal_to(truth));
    });

    // _.test("factory functions", [all_values]() {

    //     std::vector<value> v {
    //         value::whole(),
    //         value::half(),
    //         value::quarter(),
    //         value::eighth(),
    //         value::sixteenth(),
    //         value::thirtysecond(),
    //         value::sixtyfourth(),
    //         //value::whole(dot(1)),
    //         //value::half(dot(1)),
    //         //value::quarter(dot(1)),
    //         //value::eighth(dot(1)),
    //         //value::sixteenth(dot(1)),
    //         //value::thirtysecond(dot(1)),
    //         //value::whole(dot(2)),
    //         //value::half(dot(2)),
    //         //value::quarter(dot(2)),
    //         //value::eighth(dot(2)),
    //         //value::sixteenth(dot(2))
    //     };

    //     expect(v, each(all_values, equal_to<const value&>));
    // });
});
