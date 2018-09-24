#include <stan/column.hpp>
#include <stan/driver/debug.hpp>
#include <stan/driver/lilypond.hpp>

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
    m_variant(std::move(copy_variant()(c.m_variant).m_variant))
{
    std::cout << "column::copy constructor " << stan::driver::debug::write(c);
}

column column::operator=(const column &c)
{
    std::cout << "column::operator= " << stan::driver::debug::write(c);
    m_variant = copy_variant()(c.m_variant).m_variant;
    return copy_variant()(c);
    // return column(std::move(copy_variant()(c.m_variant).m_variant));
};

template <typename T>
auto copy_variant::operator()(const T &v) const -> decltype(column{ v })
{
    std::cout << "direct copy " << typeid(T).name() << " " << driver::debug::write(v) << std::endl;
    return v;
}

column copy_variant::operator()(const std::unique_ptr<column> &v) const
{
    std::cout << "unique_ptr copy " << driver::debug::write(v) << std::endl;
    column c = (*this)(v->m_variant);
    return std::make_unique<column>(std::move(c));
}

column copy_variant::operator()(const column &v) const
{
    std::cout << "column copy " << driver::debug::write(v) << " " << driver::debug::write(std::visit(*this, v.m_variant)) << std::endl;
    return std::visit(*this, v.m_variant);
}

template column copy_variant::operator()<rest>(const rest &) const;
template column copy_variant::operator()<note>(const note &) const;
template column copy_variant::operator()<chord>(const chord &) const;
// template column copy_variant::operator()<beam>(const beam &) const;
// template column copy_variant::operator()<beam>(const beam &) const;
// template column copy_variant::operator()<lilypond::default_ctor<beam>>(const lilypond::default_ctor<beam> &) const;
} // namespace stan
