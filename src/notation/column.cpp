#include <stan/column.hpp>
#include <stan/duration.hpp>
#include <stan/driver/debug.hpp>
#include <stan/driver/lilypond.hpp>

#include <numeric>

namespace stan {

int operator==(const rest &c1, const rest &c2)
{
    return boost::hana::equal(c1, c2);
}

int operator==(const pitch &c1, const pitch &c2)
{
    return boost::hana::equal(c1, c2);
}

int operator==(const note &c1, const note &c2)
{
    return boost::hana::equal(c1, c2);
}

int operator==(const chord &c1, const chord &c2)
{
    return boost::hana::equal(c1, c2);
}

int operator==(const beam &c1, const beam &c2)
{
    return boost::hana::equal(c1, c2);
}

int operator==(const tuplet &c1, const tuplet &c2)
{
    return boost::hana::equal(c1, c2);
}

bool operator==(const column &c1, const column &c2)
{
    return c1.m_variant == c2.m_variant;
}

bool operator==(std::unique_ptr<column> const &c1, std::unique_ptr<column> const &c2)
{
    return c1->m_variant == c2->m_variant;
}

column::column(const column &c) :
    m_variant(std::move(copy_variant()(c.m_variant)))
{
    // std::cout << "column::copy constructor " << stan::driver::debug::write(c);
}

void column::operator=(const column &c)
{
    std::cout << "column::operator= got " << stan::driver::debug::write(c);
    m_variant = copy_variant()(c.m_variant);
    std::cout << "column::operator= is now " << stan::driver::debug::write(*this) << std::endl;
    // return column(std::move(copy_variant()(c.m_variant).m_variant));
};

struct get_duration
{
    duration operator()(rest const &v) const { return v.m_value; }
    duration operator()(note const &v) const { return v.m_value; }
    duration operator()(chord const &v) const { return v.m_value; }
    duration operator()(beam const &v) const
    {
        return std::accumulate(
            v.m_elements.begin(),
            v.m_elements.end(),
            duration::zero(),
            [](duration res, const auto &p) { return res + p; });
    }
    duration operator()(tuplet const &v) const { return v.m_value; }
    duration operator()(std::unique_ptr<column> const &v) const
    {
        return std::visit(*this, v->m_variant);
    }
};

duration operator+(stan::duration const &d, stan::column const &c)
{
    return d + std::visit(get_duration(), c.m_variant);
}

template <typename T>
variant copy_variant::operator()(const T &v) const
{
    // std::cout << "direct copy " << typeid(T).name() << " " << driver::debug::write(v) << std::endl;
    return v;
}

variant copy_variant::operator()(const std::unique_ptr<column> &v) const
{
    // std::cout << "unique_ptr copy " << driver::debug::write(v) << std::endl;
    variant new_v = (*this)(v->m_variant);
    return std::make_unique<column>(std::move(new_v));
}

variant copy_variant::operator()(const beam &v) const
{
    std::vector<column> elements;
    std::transform(
        v.m_elements.begin(),
        v.m_elements.end(),
        std::back_inserter(elements),
        [this](const column &c) { return column(std::visit(*this, c.m_variant)); });
    return beam{ elements };
}

variant copy_variant::operator()(const tuplet &v) const
{
    std::vector<column> elements;
    std::transform(
        v.m_elements.begin(),
        v.m_elements.end(),
        std::back_inserter(elements),
        [this](const column &c) { return column(std::visit(*this, c.m_variant)); });
    return tuplet{ v.m_value, std::move(elements) };
}

variant copy_variant::operator()(const column &v) const
{
    // std::cout << "column copy " << driver::debug::write(v) << " " << driver::debug::write(std::visit(*this, v.m_variant)) << std::endl;
    return std::visit(*this, v.m_variant);
}

template variant copy_variant::operator()<rest>(const rest &) const;
template variant copy_variant::operator()<note>(const note &) const;
template variant copy_variant::operator()<chord>(const chord &) const;

value tuplet::scale(int num, int den, const duration &inner)
{
    stan::duration outer(inner.num() * den, inner.den() * num);

    for (const stan::value &val : stan::value::all) {
        if (outer == static_cast<duration>(val)) {
            return val;
        }
    }
    throw invalid_value("tuplet duration ({}/{}:{{{}}} = {}/{}) must equal a valid value",
                        num, den, stan::driver::debug::write(inner),
                        outer.num(), outer.den());
}

value tuplet::scale(int num, int den, const value &inner)
{
    return tuplet::scale(num, den, static_cast<duration>(inner));
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
                return "cannot hold whole or half notes";
            }
            if (m_numelements < 2) {
                return "must contain at least two values";
            }
            return std::string();
        }

        std::string operator()(chord const &v) const
        {
            if (v.m_value > value::quarter()) {
                return "cannot hold whole or half notes";
            }
            if (m_numelements < 2) {
                return "must contain at least two values";
            }
            return std::string();
        }

        std::string operator()(beam const &v) const
        {
            if (m_numelements < 2) {
                return "nested beams must contain at least two values";
            }
            return std::string();
        }

        std::string operator()(tuplet const &v) const
        {
            if (v.m_value > value::quarter()) {
                return "cannot hold whole or half note tuplets";
            }
            return std::string();
        }

        std::string operator()(std::unique_ptr<column> const &v) const
        {
            return std::visit(*this, v->m_variant);
        };
    };

    std::for_each(
        m_elements.begin(),
        m_elements.end(),
        [numelements = m_elements.size()](auto &c) {
            std::string result = std::visit(is_valid{ numelements }, c.m_variant);
            if (!result.empty()) {
                throw invalid_beam("{}", result);
            }
        });
}

} // namespace stan
