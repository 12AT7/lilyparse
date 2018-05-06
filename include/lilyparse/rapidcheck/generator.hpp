#pragma once

#include <lilyparse/pitch.hpp>

#include <rapidcheck.h>

namespace rc {

using namespace lilyparse;

template <>
struct Arbitrary<pitchclass>
{
    static Gen<pitchclass> arbitrary()
    {
        static valid_pitchclass valid;
        return gen::elementOf(valid);
    }
};

template <>
struct Arbitrary<octave>
{
    static Gen<octave> arbitrary()
    {
        return gen::construct<octave>(gen::inRange<std::uint8_t>(0, 7));
    }
};

template <>
struct Arbitrary<pitch>
{
    static Gen<pitch> arbitrary()
    {
        return gen::build<pitch>(
            gen::set(&pitch::pitchclass_, gen::arbitrary<pitchclass>()),
            gen::set(&pitch::octave_, gen::arbitrary<octave>()));
    };
};

} // namespace rc
