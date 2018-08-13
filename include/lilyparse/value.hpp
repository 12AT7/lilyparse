#pragma once

#include "rational.hpp"
#include "generate.hpp"
#include "exception.hpp"

#include <vector>

namespace lilyparse {

struct duration;

struct value : rational<std::uint16_t>
{
  public:
    static value whole() { return { 1, 1 }; }
    static value half() { return { 1, 2 }; }
    static value quarter() { return { 1, 4 }; }
    static value eighth() { return { 1, 8 }; }
    static value sixteenth() { return { 1, 16 }; }
    static value thirtysecond() { return { 1, 32 }; }
    static value sixtyfourth() { return { 1, 64 }; }

    operator duration() const;
    using dots_t = std::uint8_t;
    dots_t dots() const;

    using rational<std::uint16_t>::rational;

    // The free function dot() needs the constructor.
    friend value dot(const value &v);

    static const std::vector<value> all;

};

value dot(const value &v)
{
    if (v.num() != 1 and v.num() != 3)
        throw invalid_value("values can have exactly 0, 1, or 2 dots");
    return {
        static_cast<value::integer>(2 * v.num() + 1),
        static_cast<value::integer>(2 * v.den())
    };
}

template <>
struct string_generator<value>
{
    std::string operator()(value const &v)
    {
        return std::to_string(v.den() / (1u << v.dots())) + std::string(v.dots(), '.');
    }
};

// struct tie : std::deque<value>
// {
// };

// struct instant : rational<std::uint64_t>
// {
// };

} // namespace lilyparse
