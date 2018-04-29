#pragma once

#include <type_safe/strong_typedef.hpp>
#include <vector>

namespace lilyparse {

// Unlike pitches, rythymic values *are* ammenable to sensible calculations
// using integer shifts and counting dots.  However, there are so few *actual*
// valid values used in real music that the model is made much simpler by just
// fully enumerating the possibilities.

enum struct value : std::uint8_t
{
    whole,
    whole_dot,
    whole_dotdot,
    half,
    half_dot,
    half_dotdot,
    quarter,
    quarter_dot,
    quarter_dotdot,
    eighth,
    eighth_dot,
    eighth_dotdot,
    sixteenth,
    sixteenth_dot,
    sixteenth_dotdot,
    thirtysecond,
    thirtysecond_dot,
    sixtyfourth,
    instantaneous
};

extern const std::vector<value> all_values;
extern std::uint8_t dots(value);
extern std::string to_string(value);
std::ostream &operator<<(std::ostream &os, value v)
{
    return os << to_string(v);
}

namespace ts = type_safe;

struct duration : ts::strong_typedef<duration, std::uint16_t>,
                  ts::strong_typedef_op::equality_comparison<duration>,
                  ts::strong_typedef_op::addition<duration>
{
    using ts::strong_typedef<duration, std::uint16_t>::strong_typedef;

    duration(value);
};

struct instant : ts::strong_typedef<duration, std::uint32_t>,
                 ts::strong_typedef_op::equality_comparison<instant>,
                 ts::strong_typedef_op::relational_comparison<duration>,
                 ts::strong_typedef_op::mixed_addition<instant, duration>
{
    using ts::strong_typedef<duration, std::uint32_t>::strong_typedef;
};

} // namespace lilyparse
