#pragma once

#include <stan/pitch.hpp>
#include <stan/value.hpp>
#include <stan/column.hpp>

#include <rapidcheck.h>

namespace rc {

using namespace stan;

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
        static valid_pitchclass valid;
        return gen::construct<pitch>(
            gen::elementOf(valid),
            gen::arbitrary<octave>());
    };
};

template <>
struct Arbitrary<value>
{
    static Gen<value> arbitrary()
    {
        return gen::elementOf(value::all);
    };
};

template <>
struct Arbitrary<note>
{
    static Gen<note> arbitrary()
    {
        return gen::construct<note>(
                gen::arbitrary<value>(),
                gen::arbitrary<pitch>()
                );
    };
};

} // namespace rc
