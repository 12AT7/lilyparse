#pragma once

#include <stan/column.hpp>

namespace stan::driver::debug {

struct writer
{
    std::string operator()(column const &);
    std::string operator()(duration const &);
    std::string operator()(value const &);
    std::string operator()(note const &);
    std::string operator()(rest const &);
    std::string operator()(chord const &);
    std::string operator()(pitch const &);
    std::string operator()(std::unique_ptr<column> const &);

  private:
    // Disable implicit conversions.  This helps avoid both bad error messages
    // when unhandled types get converted to column, and ambiguous function
    // calls when column always matches every argument in addition to the
    // correct overload.  When the code is complete and correct, this template
    // is not needed.  It is helpful when developing, though.

    template <typename T>
    std::string operator()(T const &);
};

} // namespace stan::driver::debug
