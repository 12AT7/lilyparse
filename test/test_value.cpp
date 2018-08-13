#include <mettle.hpp>
#include <lilyparse/rapidcheck/mettle.hpp>

#include <lilyparse/value.hpp>
#include <lilyparse/duration.hpp>

#include <numeric>

namespace snm = lilyparse;

namespace snm_test {

// Need a duration type that can be arbitrarily constructed, for tests
struct duration : snm::duration 
{
    duration(std::uint32_t num, std::uint32_t den) : snm::duration(num, den) {}
    using snm::duration::duration;
};

} // snm_test

namespace lilyparse {

std::string to_printable(duration const& value) 
{
    static snm::string_generator<duration> generate;
    return generate(value);
}

} // snm

using mettle::equal_to;
using mettle::expect;

mettle::suite<> suite("simple values", [](auto &_) {

        _.test("generate string", []() {
   	    snm::string_generator<std::vector<snm::value>> generate;
            expect(generate(snm::value::all),
                equal_to("1 2 4 8 16 32 64 "
                         "1. 2. 4. 8. 16. 32. "
                         "1.. 2.. 4.. 8.. 16.."));
        });

        _.test("count dots", []() {
            std::vector<snm::value::dots_t> truth {
                0, 0, 0, 0, 0, 0, 0,
                1, 1, 1, 1, 1, 1,
                2, 2, 2, 2, 2
            };
            std::vector<snm::value::dots_t> dots(snm::value::all.size());
            std::transform(snm::value::all.begin(), snm::value::all.end(), dots.begin(),
                [](snm::value v) { return v.dots(); });
            expect(dots, equal_to(truth));
        });

        _.test("duration", []() {

       	    // Use operator snm::value::duration to extract durations
            std::vector<snm::duration> durations; 
            std::copy(snm::value::all.begin(), 
	              snm::value::all.end(), 
	              std::back_inserter(durations));

	    std::vector<snm_test::duration> truth {
                { 1, 1 }, { 1, 2 }, { 1, 4 }, { 1, 8 }, { 1, 16 }, { 1, 32 }, { 1, 64 },
                { 3, 2 }, { 3, 4 }, { 3, 8 }, { 3, 16 }, { 3, 32 }, { 3, 64 },
                { 7, 4 }, { 7, 8 }, { 7, 16 }, { 7, 32 }, { 7, 64 }
            };

	    auto match = mettle::equal_to<const snm::duration&>;
	    expect(durations, mettle::each(truth.begin(), truth.end(), match));
        });
    });
