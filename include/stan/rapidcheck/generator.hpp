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

template <typename T>
struct Arbitrary<rational<T>>
{
    static Gen<rational<T>> arbitrary()
    {
        // Restrict rationals to those that are roughly interesting for tuplet ratios.
        return gen::suchThat(
            gen::apply(rational<T>::unsafe,
                       gen::inRange<T>(2, 16).as("numerator"),
                       gen::inRange<T>(2, 16).as("denominator")),
            [](auto r) { return r.num() > 1 and r.den() > 1; });
    }
};

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
struct Arbitrary<tuplet>
{
    static Gen<tuplet> arbitrary()
    {
        // clang-format off
        auto simple = gen::apply([](value v, int n, pitch p) {
            value dim = dimin(v);
            std::vector<column> elements = std::vector<column>(n, note(dim, p));
            return tuplet(v, elements);
        },
        gen::element(value::eighth(), value::quarter(), value::half()), 
	gen::inRange(3, 12), gen::arbitrary<pitch>()
	);

        auto compound2 = gen::apply([](value v, pitch p) {
            return tuplet(dot(v), note(v, p), note(v, p));
        },
        gen::element(value::eighth(), value::quarter(), value::half()), 
	gen::arbitrary<pitch>()
	);

        auto compound4 = gen::apply([](value v, pitch p) {
            return tuplet(dot(v), std::vector<column>(4, note(dimin(v), p)));
        },
        gen::element(value::eighth(), value::quarter(), value::half()), 
	gen::arbitrary<pitch>()
	);

        return gen::weightedOneOf<tuplet>({
        	{ 4, simple }, 
		{ 2, compound2 },
		{ 1, compound4 }
		});
        // clang-format on
    }
};

template <>
struct Arbitrary<beam>
{
    static Gen<beam> arbitrary()
    {
        auto has_flag = [](auto &&n) { return n.m_value < value::quarter(); };
        auto has_beam = [](auto &&n) { return n.m_value <= value::quarter(); };
        return gen::construct<beam>(
            gen::oneOf(
                gen::atLeastTwo(gen::container<std::vector<column>>(
                    gen::oneOf(
                        gen::cast<column>(gen::suchThat<note>(has_flag)),
                        gen::cast<column>(gen::suchThat<chord>(has_flag)))))
                // gen::container<std::vector<column>>(1, gen::suchThat<tuplet>(has_beam))
                ));
    };
};

} // namespace rc
