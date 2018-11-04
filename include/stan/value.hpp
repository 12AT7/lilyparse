#pragma once

#include "rational.hpp"
#include "exception.hpp"

#include <vector>

namespace stan {

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
    static value instantaneous() { return { 0, 1 }; }

    operator duration() const;
    using dots_t = std::uint8_t;
    dots_t dots() const;

    using rational<std::uint16_t>::rational;

    // The free function dot() needs the constructor.
    friend value dot(const value &v);
    friend value dimin(const value &v);
    friend value augment(const value &v);
    friend duration operator*(int, value const &);

    // friend bool operator==(const value &, const value &);
    static const std::vector<value> all;
};

value dot(const value &v);
value dimin(const value &v);
value augment(const value &v);

} // namespace stan
