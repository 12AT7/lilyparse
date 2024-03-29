#include <stan/notation.hpp>
#include <stan/driver/debug.hpp>

#include <mettle.hpp>

namespace stan {

static stan::driver::debug::writer write;

template <typename T>
std::string to_printable(rational<T> const &ev)
{
    return write(ev);
}

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

std::string to_printable(meter const &ev)
{
    return write(ev);
}

std::string to_printable(clef const &ev)
{
    return write(ev);
}

std::string to_printable(key const &ev)
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
