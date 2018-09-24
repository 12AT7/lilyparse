#pragma once

#include <stan/pitch.hpp>
#include <stan/value.hpp>
#include <stan/column.hpp>

#include <rapidcheck.h>
#include "rapidcheck/shrinkable/Create.hpp"

namespace rc {

using namespace stan;

namespace gen {

template <typename T>
Gen<T> atLeastTwo(Gen<T> gen)
{
    return gen::suchThat(std::move(gen), [](const T &x) { return x.size() >= 2; });
}

} // namespace gen

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
            gen::arbitrary<pitch>());
    };
};

template <>
struct Arbitrary<rest>
{
    static Gen<rest> arbitrary()
    {
        return gen::construct<rest>(gen::arbitrary<value>());
    };
};

template <>
struct Arbitrary<chord>
{
    static Gen<chord> arbitrary()
    {
        return gen::construct<chord>(
            gen::arbitrary<value>(),
            gen::atLeastTwo(gen::unique<std::vector<pitch>>(gen::arbitrary<pitch>())));
    };
};

template <>
struct Arbitrary<column>
{
    static Gen<column> arbitrary()
    {
        return gen::construct<column>(gen::arbitrary<note>());
    };
};

template <>
struct Arbitrary<beam>
{
    static Gen<beam> arbitrary()
    {
        return gen::construct<beam>(
            gen::atLeastTwo(gen::container<std::vector<column>>(gen::arbitrary<column>())));
    };
};

template <>
struct Arbitrary<tuplet>
{
    static Gen<tuplet> arbitrary()
    {
        return gen::construct<tuplet>(
            gen::arbitrary<value>(),
            gen::atLeastTwo(gen::container<std::vector<column>>(gen::arbitrary<column>())));
    };
};

} // namespace rc
