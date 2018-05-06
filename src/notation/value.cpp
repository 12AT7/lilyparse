#include <lilyparse/value.hpp>

#include <map>

namespace lilyparse {

// clang-format is broken on this pattern, or I am clueless how to configure.
// 8-(
const std::vector<value> all_values{ value::whole, value::half, value::quarter,
    value::eighth, value::sixteenth, value::thirtysecond, value::sixtyfourth,
    value::whole_dot, value::half_dot, value::quarter_dot, value::eighth_dot,
    value::sixteenth_dot, value::thirtysecond_dot, value::whole_dotdot,
    value::half_dotdot, value::quarter_dotdot, value::eighth_dotdot,
    value::sixteenth_dotdot };

static const std::map<value, std::uint16_t> duration_lookup{
    { value::instantaneous, 0 },
    { value::sixtyfourth, 1 },
    { value::thirtysecond, 2 },
    { value::thirtysecond_dot, 3 },
    { value::sixteenth, 4 },
    { value::sixteenth_dot, 6 },
    { value::sixteenth_dotdot, 7 },
    { value::eighth, 8 },
    { value::eighth_dot, 12 },
    { value::eighth_dotdot, 14 },
    { value::quarter, 16 },
    { value::quarter_dot, 24 },
    { value::quarter_dotdot, 28 },
    { value::half, 32 },
    { value::half_dot, 48 },
    { value::half_dotdot, 56 },
    { value::whole, 64 },
    { value::whole_dot, 96 },
    { value::whole_dotdot, 112 },
};

duration::duration(value v)
    : ts::strong_typedef<duration, std::uint16_t>(duration_lookup.at(v))
{
}

std::string to_string(value v)
{
    static const std::map<value, const char *> lookup{
        { value::instantaneous, "" },
        { value::sixtyfourth, "64" },
        { value::thirtysecond, "32" },
        { value::thirtysecond_dot, "32." },
        { value::sixteenth, "16" },
        { value::sixteenth_dot, "16." },
        { value::sixteenth_dotdot, "16.." },
        { value::eighth, "8" },
        { value::eighth_dot, "8." },
        { value::eighth_dotdot, "8.." },
        { value::quarter, "4" },
        { value::quarter_dot, "4." },
        { value::quarter_dotdot, "4.." },
        { value::half, "2" },
        { value::half_dot, "2." },
        { value::half_dotdot, "2.." },
        { value::whole, "1" },
        { value::whole_dot, "1." },
        { value::whole_dotdot, "1.." },
    };

    return lookup.at(v);
}

std::uint8_t dots(value v)
{
    static const std::map<value, std::uint8_t> lookup{
        { value::instantaneous, 0 },
        { value::sixtyfourth, 0 },
        { value::thirtysecond, 0 },
        { value::thirtysecond_dot, 1 },
        { value::sixteenth, 0 },
        { value::sixteenth_dot, 1 },
        { value::sixteenth_dotdot, 2 },
        { value::eighth, 0 },
        { value::eighth_dot, 1 },
        { value::eighth_dotdot, 2 },
        { value::quarter, 0 },
        { value::quarter_dot, 1 },
        { value::quarter_dotdot, 2 },
        { value::half, 0 },
        { value::half_dot, 1 },
        { value::half_dotdot, 2 },
        { value::whole, 0 },
        { value::whole_dot, 1 },
        { value::whole_dotdot, 2 },
    };

    return lookup.at(v);
}

} // namespace lilyparse
