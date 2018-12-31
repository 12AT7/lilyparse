#include <stan/notation.hpp>
#include <stan/notation/equal.hpp>

#include <stan/driver/debug.hpp>
#include <stan/driver/lilypond.hpp>

#include <numeric>

namespace stan {

struct get_duration
{
    duration operator()(rest const &v) const { return v.m_value; }
    duration operator()(note const &v) const { return v.m_value; }
    duration operator()(chord const &v) const { return v.m_value; }
    duration operator()(tuplet const &v) const { return v.m_value; }
    duration operator()(beam const &v) const
    {
        return std::accumulate(
            v.m_elements.begin(),
            v.m_elements.end(),
            duration::zero(),
            [](duration res, const auto &p) { return res + p; });
    }

    template <typename C>
    duration operator()(C const& v) const { return duration::zero(); }
};

duration operator+(stan::duration const &d, stan::column const &c)
{
    return d + std::visit(get_duration(), c);
}

value tuplet::scale(int num, int den, const duration &inner)
{
    stan::duration outer(inner.num() * den, inner.den() * num);

    for (const stan::value &val : stan::value::all) {
        if (outer == static_cast<duration>(val)) {
            return val;
        }
    }
    throw invalid_tuplet("duration ({}/{}:{{{}}} = {}/{}) must equal a valid value",
                         num, den, stan::driver::debug::write(inner),
                         outer.num(), outer.den());
}

value tuplet::scale(int num, int den, const value &inner)
{
    return tuplet::scale(num, den, static_cast<duration>(inner));
}

void tuplet::validate() const
{
    if (m_elements.size() < 2) {
        throw invalid_tuplet("must contain at least two elements");
    }
}

void meter::validate() const
{
    static std::set<value> valid_values{
        value::half(),
        value::quarter(),
        value::eighth(),
        value::sixteenth(),
        value::thirtysecond()
    };

    if (m_beats.empty()) {
        throw invalid_meter("no beats");
    }
    if (valid_values.count(m_value) == 0) {
        throw invalid_meter(
            "value must be half, quarter, eighth, sixteenth, or thirtysecond");
    }
}

void beam::validate() const
{
    struct is_valid
    {
        size_t m_numelements;

        std::string operator()(rest const &v) const
        {
            return "cannot contain rests";
        }

        std::string operator()(note const &v) const
        {
            if (v.m_value > value::quarter()) {
                return "cannot contain whole or half notes";
            }
            if (m_numelements < 2) {
                return "must contain at least two elements";
            }
            return std::string();
        }

        std::string operator()(chord const &v) const
        {
            if (v.m_value > value::quarter()) {
                return "cannot contain whole or half notes";
            }
            if (m_numelements < 2) {
                return "must contain at least two elements";
            }
            return std::string();
        }

        std::string operator()(beam const &v) const
        {
            if (m_numelements < 2) {
                return "nested beams must contain at least two elements";
            }
            return std::string();
        }

        std::string operator()(tuplet const &v) const
        {
            if (v.m_value > value::quarter()) {
                return "cannot contain whole or half note tuplets";
            }
            return std::string();
        }
    };

    std::for_each(
        m_elements.begin(),
        m_elements.end(),
        [numelements = m_elements.size()](auto &c) {
            std::string result = std::visit(is_valid{ numelements }, c);
            if (!result.empty()) {
                throw invalid_beam("{}", result);
            }
        });
}

} // namespace stan
