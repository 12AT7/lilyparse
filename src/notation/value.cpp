#include <stan/value.hpp>
#include <stan/duration.hpp>

#include <map>

namespace stan {

value dot(const value &v)
{
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
    dot(dot(sixteenth())),
};

// static const std::map<value, std::uint16_t> duration_lookup{
value::operator duration() const
{
    return { 0, 0 };
}

//     { value::instantaneous, 0 },
//     { value::sixtyfourth, 1 },
//     { value::thirtysecond, 2 },
//     { value::thirtysecond_dot, 3 },
//     { value::sixteenth, 4 },
//     { value::sixteenth_dot, 6 },
//     { value::sixteenth_dotdot, 7 },
//     { value::eighth, 8 },
//     { value::eighth_dot, 12 },
//     { value::eighth_dotdot, 14 },
//     { value::quarter, 16 },
//     { value::quarter_dot, 24 },
//     { value::quarter_dotdot, 28 },
//     { value::half, 32 },
//     { value::half_dot, 48 },
//     { value::half_dotdot, 56 },
//     { value::whole, 64 },
//     { value::whole_dot, 96 },
//     { value::whole_dotdot, 112 },
// };
//
// duration::duration(value v)
//     : ts::strong_typedef<duration, std::uint16_t>(duration_lookup.at(v))
// {
// }
//
// std::string to_string(value v)
// {
//     static const std::map<value, const char *> lookup{
//         { value::instantaneous, "" },
//         { value::sixtyfourth, "64" },
//         { value::thirtysecond, "32" },
//         { value::thirtysecond_dot, "32." },
//         { value::sixteenth, "16" },
//         { value::sixteenth_dot, "16." },
//         { value::sixteenth_dotdot, "16.." },
//         { value::eighth, "8" },
//         { value::eighth_dot, "8." },
//         { value::eighth_dotdot, "8.." },
//         { value::quarter, "4" },
//         { value::quarter_dot, "4." },
//         { value::quarter_dotdot, "4.." },
//         { value::half, "2" },
//         { value::half_dot, "2." },
//         { value::half_dotdot, "2.." },
//         { value::whole, "1" },
//         { value::whole_dot, "1." },
//         { value::whole_dotdot, "1.." },
//     };
//
//     return lookup.at(v);
// }
//
value::dots_t value::dots() const
{
    static const std::map<value, value::dots_t> lookup{
        { { value::sixtyfourth(), 0 },
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

    return lookup.at(*this);
}

} // namespace stan
