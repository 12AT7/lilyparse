#pragma once

#include <stan/notation.hpp>
#include <stan/exception.hpp>

#include <boost/hana/define_struct.hpp>

#include <numeric>

namespace stan {

struct invalid_tuplet : exception
{
    template <typename... Args>
    invalid_tuplet(const char *format, Args... args) :
        exception((std::string("invalid tuplet: ") + format).c_str(),
                  std::forward<Args>(args)...) {}
};

struct tuplet
{
    BOOST_HANA_DEFINE_STRUCT(tuplet,
                             (value, m_value),
                             (std::vector<column>, m_elements));

    template <typename Element>
    tuplet(const value &v, const std::vector<Element> &n) :
        m_value(v)
    {
        std::copy(n.begin(), n.end(), std::back_inserter(m_elements));
        validate();
    }

    template <typename... VoiceElement>
    tuplet(const value &v, VoiceElement... element) :
        m_value(v)
    {
        (m_elements.emplace_back(element), ...);
        validate();
    }

    static value scale(int num, int den, duration const &inner);
    static value scale(int num, int den, value const &inner);

    template <typename ElementContainer>
    static value scale(int num, int den, ElementContainer const &elements);

    operator duration() const { return m_value; }

  private:
    void validate() const;
};

template <typename ElementContainer>
value tuplet::scale(int num, int den, ElementContainer const &elements)
{
    duration inner = std::accumulate(
        elements.begin(),
        elements.end(),
        duration::zero(),
        [](duration res, const auto &p) { return res + p; });

    return tuplet::scale(num, den, inner);
}

}

