#include <mettle.hpp>
#include <stan/rapidcheck/mettle.hpp>

#include <stan/value.hpp>
#include <stan/duration.hpp>

#include <numeric>

namespace stan_test {

// Need a duration type that can be arbitrarily constructed, for tests
struct duration : stan::duration
{
    duration(std::uint32_t num, std::uint32_t den) :
        stan::duration(num, den) {}
    using stan::duration::duration;
};

} // namespace stan_test

namespace stan {

std::string to_printable(duration const &value)
{
    static stan::string_generator<duration> generate;
    return generate(value);
}

} // namespace stan

using mettle::equal_to;
using mettle::expect;

mettle::suite<> suite("simple values", [](auto &_) {
    _.test("generate string", []() {
        stan::string_generator<std::vector<stan::value>> generate;
        expect(generate(stan::value::all),
               equal_to("1 2 4 8 16 32 64 "
                        "1. 2. 4. 8. 16. 32. "
                        "1.. 2.. 4.. 8.. 16.."));
    });

    _.test("count dots", []() {
        std::vector<stan::value::dots_t> truth{
            0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1,
            2, 2, 2, 2, 2
        };
        std::vector<stan::value::dots_t> dots(stan::value::all.size());
        std::transform(stan::value::all.begin(), stan::value::all.end(), dots.begin(),
                       [](stan::value v) { return v.dots(); });
        expect(dots, equal_to(truth));
    });

    _.test("duration", []() {
        // Use operator stan::value::duration to extract durations
        std::vector<stan::duration> durations;
        std::copy(stan::value::all.begin(),
                  stan::value::all.end(),
                  std::back_inserter(durations));

        std::vector<stan_test::duration> truth{
            { 1, 1 }, { 1, 2 }, { 1, 4 }, { 1, 8 }, { 1, 16 }, { 1, 32 }, { 1, 64 }, { 3, 2 }, { 3, 4 }, { 3, 8 }, { 3, 16 }, { 3, 32 }, { 3, 64 }, { 7, 4 }, { 7, 8 }, { 7, 16 }, { 7, 32 }, { 7, 64 }
        };

        auto match = mettle::equal_to<const stan::duration &>;
        expect(durations, mettle::each(truth.begin(), truth.end(), match));
    });
});
