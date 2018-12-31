#pragma once

#include <stan/notation.hpp>

#include <boost/variant.hpp>

// Deep copies of notation elements.  Without these, lilypond_reader crashes,
// even though it can be coaxed to compile without the deep copies.  This is a
// bit mysterious yet; it probably has to do with some subtlety of spirit::x3
// copying references instead of values, by default, or else some gotcha with
// boost::variant (used only by spirit::x3 in stan).  Never could figure this
// out, so this deep copy template is the workaround which works fine.

namespace stan {

struct copy_visitor
{
    template <typename T>
    column operator()(const T &v) const;

    column operator()(const tuplet &v) const;
    column operator()(const beam &v) const;

    template <typename... Ts>
    column operator()(const boost::variant<Ts...> &v) const;

    template <typename... Ts>
    column operator()(const std::variant<Ts...> &v) const;
};

template <typename... Ts>
inline column copy_visitor::operator()(const boost::variant<Ts...> &v) const
{
    return boost::apply_visitor(*this, v);
}

template <typename... Ts>
inline column copy_visitor::operator()(const std::variant<Ts...> &v) const
{
    return std::visit(*this, v);
}

template <typename T>
inline column copy(const T& v) { return copy_visitor()(v); }
}

