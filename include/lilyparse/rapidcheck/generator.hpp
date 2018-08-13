#pragma once

#include <lilyparse/pitch.hpp>

#include <rapidcheck.h>

namespace snm = lilyparse;

namespace rc {

template <>
struct Arbitrary<snm::pitchclass>
{
    static Gen<snm::pitchclass> arbitrary()
    {
        static snm::valid_pitchclass valid;
        return gen::elementOf(valid);
    }
};

template <>
struct Arbitrary<snm::octave>
{
    static Gen<snm::octave> arbitrary()
    {
        return gen::construct<snm::octave>(gen::inRange<std::uint8_t>(0, 7));
    }
};

template <>
struct Arbitrary<snm::pitch>
{
    static Gen<snm::pitch> arbitrary()
    {
        return gen::build<snm::pitch>(
            gen::set(&snm::pitch::pitchclass_, gen::arbitrary<snm::pitchclass>()),
            gen::set(&snm::pitch::octave_, gen::arbitrary<snm::octave>()));
    };
};

} // namespace rc
