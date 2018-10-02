#include <stan/driver/debug.hpp>
#include <stan/column.hpp>

#include <mettle.hpp>

namespace stan {

static stan::driver::debug::writer write;

std::string to_printable(value const &ev)
{
    return write(ev);
}

std::string to_printable(pitch const &ev)
{
    return write(ev);
}

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

std::string to_printable(beam const &ev)
{
    return write(ev);
}

std::string to_printable(tuplet const &ev)
{
    return write(ev);
}

std::string to_printable(column const &ev)
{
    return write(ev);
}

// Rapidcheck, of course, uses a similar but not the same custom printer as Mettle.
template <typename T>
void showValue(const T &value, std::ostream &os)
{
    os << to_printable(value);
}

} // namespace stan
