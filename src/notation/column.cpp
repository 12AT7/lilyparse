#include <stan/column.hpp>
#include <stan/driver/debug.hpp>

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

bool operator==(const column &c1, const column &c2)
{
    stan::driver::debug::writer write;
    return c1.m_variant == c2.m_variant;
}

bool operator==(std::unique_ptr<column> const &c1, std::unique_ptr<column> const &c2)
{
    stan::driver::debug::writer write;
    return c1->m_variant == c2->m_variant;
}

} // namespace stan
