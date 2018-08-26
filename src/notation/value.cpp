#include <stan/value.hpp>
#include <stan/duration.hpp>

#include <map>

namespace stan {

value dot(const value &v)
{
    // The operation is either going from 0->1 dot, or 1->2 dots.  There
    // are no other valid situations.
    if (v.num() != 1 and v.num() != 3) {
        throw invalid_value("values can have exactly 0, 1, or 2 dots");
    }
    return {
        static_cast<value::integer>(2 * v.num() + 1),
        static_cast<value::integer>(2 * v.den())
    };
}

const std::vector<value> value::all{
    whole(),
    half(),
    quarter(),
    eighth(),
    sixteenth(),
    thirtysecond(),
    sixtyfourth(),
    dot(whole()),
    dot(half()),
    dot(quarter()),
    dot(eighth()),
    dot(sixteenth()),
    dot(thirtysecond()),
    dot(dot(whole())),
    dot(dot(half())),
    dot(dot(quarter())),
    dot(dot(eighth())),
    dot(dot(sixteenth()))
};

value::operator duration() const
{
    static const std::map<value, duration> duration_table{
        {
            { value::instantaneous(), { 0, 1 } },
            { value::sixtyfourth(), { 1, 64 } },
            { value::thirtysecond(), { 1, 32 } },
            { dot(value::thirtysecond()), { 3, 64 } },
            { value::sixteenth(), { 1, 16 } },
            { dot(value::sixteenth()), { 3, 32 } },
            { dot(dot(value::sixteenth())), { 7, 64 } },
            { value::eighth(), { 1, 8 } },
            { dot(value::eighth()), { 3, 16 } },
            { dot(dot(value::eighth())), { 7, 32 } },
            { value::quarter(), { 1, 4 } },
            { dot(value::quarter()), { 3, 8 } },
            { dot(dot(value::quarter())), { 7, 16 } },
            { value::half(), { 1, 2 } },
            { dot(value::half()), { 3, 4 } },
            { dot(dot(value::half())), { 7, 8 } },
            { value::whole(), { 1, 1 } },
            { dot(value::whole()), { 3, 2 } },
            { dot(dot(value::whole())), { 7, 4 } },
        }
    };

    return duration_table.at(*this);
}

value::dots_t value::dots() const
{
    static const std::map<value, value::dots_t> dots_table{
        { { sixtyfourth(), 0 },
          { thirtysecond(), 0 },
          { sixteenth(), 0 },
          { eighth(), 0 },
          { quarter(), 0 },
          { half(), 0 },
          { whole(), 0 },
          { dot(thirtysecond()), 1 },
          { dot(sixteenth()), 1 },
          { dot(eighth()), 1 },
          { dot(quarter()), 1 },
          { dot(half()), 1 },
          { dot(whole()), 1 },
          { dot(dot(sixteenth())), 2 },
          { dot(dot(eighth())), 2 },
          { dot(dot(quarter())), 2 },
          { dot(dot(half())), 2 },
          { dot(dot(whole())), 2 } }
    };

    return dots_table.at(*this);
}

} // namespace stan
