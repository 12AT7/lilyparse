#include <lilyparse/value.hpp>
#include <lilyparse/duration.hpp>

#include <map>

namespace lilyparse {

value::operator duration() const
{
    return { num(), den() };
}

std::uint8_t value::dots() const
{
    static const std::map<std::uint16_t, std::uint8_t>
        lookup{ { 1, 0 }, { 3, 1 }, { 7, 2 } };

    // Member num_ should only ever be 1, 3, or 7, corresponding to 0,
    // 1, and 2 dots, respectively.
    return lookup.at(num());
}

// clang-format off
const std::vector<value> value::all {
    value::whole(), 
    value::half(), 
    value::quarter(),
    value::eighth(), 
    value::sixteenth(), 
    value::thirtysecond(), 
    value::sixtyfourth(),
    dot(value::whole()), 
    dot(value::half()), 
    dot(value::quarter()), 
    dot(value::eighth()),
    dot(value::sixteenth()), 
    dot(value::thirtysecond()), 
    dot(dot(value::whole())),
    dot(dot(value::half())), 
    dot(dot(value::quarter())), 
    dot(dot(value::eighth())),
    dot(dot(value::sixteenth()))
};
// clang-format on

} // namespace lilyparse
