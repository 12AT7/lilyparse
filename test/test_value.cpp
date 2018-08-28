#include <mettle.hpp>
#include <stan/rapidcheck/mettle.hpp>

#include <stan/value.hpp>
#include <stan/duration.hpp>

#include <numeric>

namespace stan::test {

// Need a duration type that can be arbitrarily constructed, for tests
struct duration : stan::duration
{
    template <typename... Args>
    duration(Args &&... args) :
        stan::duration(std::forward<Args>(args)...) {}
};

std::string to_printable(duration const &dur)
{
    static stan::string_generator<stan::duration> generate;
    return generate(dur);
}

} // namespace stan::test

using mettle::equal_to;
using mettle::expect;
using mettle::not_equal_to;
using mettle::thrown;

mettle::suite<> suite("value", [](auto &_) {
    using stan::test::duration;

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

    _.test("too many dots", []() {
        expect([]() { dot(dot(dot(stan::value::whole()))); },
               thrown<stan::invalid_value>());
    });

    _.test("duration construction", []() {
        expect(duration(3, 8).den(), equal_to(8));
        expect(duration(3, 8).num(), equal_to(3));
        expect([]() { duration{ 1, 0 }; },
               thrown<stan::invalid_value>("invalid value: zero denominator"));
    });

    _.test("duration operators", []() {
        expect("==", duration{ 3, 8 }, equal_to(duration{ 3, 8 }));
        expect("==", duration{ 3, 8 }, equal_to(duration{ 6, 16 }));
        expect("!=", duration{ 3, 8 }, not_equal_to(duration{ 3, 4 }));
        expect("!=", duration{ 3, 8 }, not_equal_to(duration{ 4, 8 }));
        expect("add", duration{ 3, 8 } + duration{ 1, 16 }, equal_to(duration{ 7, 16 }));
    });

    _.test("value durations", []() {
        // Use operator stan::value::duration to extract durations
        std::vector<stan::duration> durations;
        std::copy(stan::value::all.begin(),
                  stan::value::all.end(),
                  std::back_inserter(durations));

        std::vector<duration> truth{
            { 1, 1 }, { 1, 2 }, { 1, 4 }, { 1, 8 }, { 1, 16 }, { 1, 32 }, { 1, 64 }, { 3, 2 }, { 3, 4 }, { 3, 8 }, { 3, 16 }, { 3, 32 }, { 3, 64 }, { 7, 4 }, { 7, 8 }, { 7, 16 }, { 7, 32 }, { 7, 64 }
        };

        auto match = mettle::equal_to<const stan::duration &>;
        expect(durations, mettle::each(truth.begin(), truth.end(), match));
    });
});

mettle::suite<> duration("duration", [](auto &_) {
    using stan::test::duration;

    _.test("to_printable", []() {
        expect(to_printable(duration{ 1, 2 }), equal_to("1/2"));
        expect(to_printable(duration{ 2, 6 }), equal_to("1/3"));
        expect(to_printable(duration{ 600, 2400 }), equal_to("1/4"));
    });

    _.test("addition", []() {
        expect(duration{ 1, 2 } + duration{ 1, 4 }, equal_to(duration{ 3, 4 }));
        expect(duration{ 3, 8 } + duration{ 5, 16 }, equal_to(duration{ 11, 16 }));
        expect(duration{ 3, 8 } + duration{ 24, 16 }, equal_to(duration{ 15, 8 }));
    });
});
