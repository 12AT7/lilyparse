#pragma once

#include <string>
#include <vector>
#include <numeric>

namespace stan {

template <typename T>
struct string_generator;

template <typename T>
struct string_generator<std::vector<T>>
{
    std::string operator()(std::vector<T> const &values)
    {
        if (values.empty())
            return {};

        static string_generator<T> generate;

        std::string result = generate(values.front());

        return std::accumulate(
            std::next(std::begin(values)),
            std::end(values),
            result,
            [](std::string const &str, T const &v) { return str + " " + generate(v); });
    }
};

} // namespace stan
