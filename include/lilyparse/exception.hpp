#pragma once

#include <fmt/format.h>

namespace lilyparse {

struct exception : public std::runtime_error
{
    template <typename... Args>
    exception(const char *format, Args... args)
        : std::runtime_error(fmt::format(format, std::forward<Args>(args)...)) {}
};

struct invalid_value : exception
{
    template <typename... Args>
    invalid_value(const char *format, Args... args)
        : exception(format, std::forward<Args>(args)...) {}
};

} // namespace lilyparse
