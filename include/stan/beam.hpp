#pragma once

#include <stan/column.hpp>
#include <stan/exception.hpp>

namespace stan {

struct invalid_beam : exception
{
    template <typename... Args>
    invalid_beam(const char *format, Args... args) :
        exception((std::string("invalid beam: ") + format).c_str(),
                  std::forward<Args>(args)...) {}
};

struct beam
{
    BOOST_HANA_DEFINE_STRUCT(beam, (std::vector<column>, m_elements));

    template <typename Element>
    beam(const std::vector<Element> &n)
    {
        std::copy(n.begin(), n.end(), std::back_inserter(m_elements));
        validate();
    }

    template <typename... VoiceElement>
    beam(VoiceElement... element)
    {
        (m_elements.emplace_back(element), ...);
        validate();
    }

  private:
    void validate() const;
};


}
