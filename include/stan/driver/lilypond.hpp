#pragma once

#include <stan/column.hpp>

#include <algorithm>

namespace stan {
}

namespace stan::lilypond {

struct writer
{
    template <typename T>
    std::string operator()(const T &) const;
};

struct reader
{
    // std::shared_ptr<column> operator()(const std::string &);
    column operator()(const std::string &);
};

} // namespace stan::lilypond
