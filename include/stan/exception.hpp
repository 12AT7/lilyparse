#pragma once

#include <fmt/format.h>

namespace stan {

struct exception : public std::runtime_error
{
    template <typename... Args>
    exception(const char *format, Args... args) :
        std::runtime_error(fmt::format(format, std::forward<Args>(args)...)) {}
};

struct invalid_value : exception
{
    template <typename... Args>
    invalid_value(const char *format, Args... args) :
        exception((std::string("invalid value: ") + format).c_str(),
                  std::forward<Args>(args)...) {}
};

struct invalid_chord : exception
{
    template <typename... Args>
    invalid_chord(const char *format, Args... args) :
        exception((std::string("invalid chord: ") + format).c_str(),
                  std::forward<Args>(args)...) {}
};

struct invalid_beam : exception
{
    template <typename... Args>
    invalid_beam(const char *format, Args... args) :
        exception((std::string("invalid beam: ") + format).c_str(),
                  std::forward<Args>(args)...) {}
};

struct invalid_tuplet : exception
{
    template <typename... Args>
    invalid_tuplet(const char *format, Args... args) :
        exception((std::string("invalid tuplet: ") + format).c_str(),
                  std::forward<Args>(args)...) {}
};

} // namespace stan
