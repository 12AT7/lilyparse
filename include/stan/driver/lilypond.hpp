#pragma once

#include <stan/column.hpp>

#include <algorithm>

namespace stan::lilypond {

struct writer
{
    template <typename T>
    std::string operator()(const T&);
};

// template <typename Event>
//Event
column parse(const std::string&);

struct reader
{
    column operator()(const std::string&);
};

} // namespace stan::lilypond
