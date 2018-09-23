#include <stan/driver/debug.hpp>
#include <stan/column.hpp>

#include <mettle.hpp>

namespace stan {

static stan::driver::debug::writer write;

std::string to_printable(rest const &ev)
{
    return write(ev);
}

std::string to_printable(note const &ev)
{
    return write(ev);
}

std::string to_printable(chord const &ev)
{
    return write(ev);
}

std::string to_printable(column const &ev)
{
    return write(ev);
}

} // namespace stan
