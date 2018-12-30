#pragma once

#include <stan/column.hpp>

namespace stan::driver::debug {

struct writer
{
    std::string operator()(rational<std::uint16_t> const &) const;
    std::string operator()(duration const &) const;
    std::string operator()(pitch const &) const;
    std::string operator()(value const &) const;
    std::string operator()(note const &) const;
    std::string operator()(rest const &) const;
    std::string operator()(chord const &) const;
    std::string operator()(beam const &) const;
    std::string operator()(tuplet const &) const;
    std::string operator()(meter const &) const;
    std::string operator()(clef const &) const;
    std::string operator()(column const &) const;
    std::string operator()(std::unique_ptr<column> const &) const;

  private:
    // Disable implicit conversions.  This helps avoid both bad error messages
    // when unhandled types get converted to column, and ambiguous function
    // calls when column always matches every argument in addition to the
    // correct overload.  When the code is complete and correct, this template
    // is not needed.  It is helpful when developing, though.

    template <typename T>
    std::string operator()(T const &) const;
};

extern writer write;

} // namespace stan::driver::debug
