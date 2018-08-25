#pragma once

#include <rapidcheck.h>
#include <mettle.hpp>
#include <stan/rapidcheck/generator.hpp>

namespace mettle {

template <typename Exception, typename Factory, typename... T>
class property_suite_builder : public suite_builder<Exception, Factory, T...>
{
    using base = suite_builder<Exception, Factory, T...>;

  public:
    using base::suite_builder;

    template <typename Testable>
    void property(std::string name, Testable &&f)
    {
        base::tests_.push_back({ name, wrap_property(f), {} });
    }

  private:
    template <typename Testable>
    auto wrap_property(Testable testable)
    {
        return [testable]() {
            const auto result = rc::detail::checkTestable(testable);

            if (result.template is<rc::detail::SuccessResult>()) {
                std::ostringstream message;
                const auto success = result.template get<rc::detail::SuccessResult>();
                if (!success.distribution.empty())
                    printResultMessage(result, message);
                return mettle::test_result{ true, message.str() };
            } else {
                std::ostringstream message;
                printResultMessage(result, message);
                throw mettle::expectation_error(message.str());
            }
        };
    }
};

template <typename Exception, typename... Fixture, typename... Args>
inline runnable_suite
make_basic_property_suite(const std::string &name, const attributes &attrs,
                          Args &&... args)
{
    using Applied = detail::apply_type<property_suite_builder, Exception>;
    return detail::do_build<Applied::template type, Fixture...>(
        name, attrs, std::forward<Args>(args)...);
}

template <typename Exception, typename... Fixture, typename... Args>
inline auto
make_basic_property_suites(const std::string &name, const attributes &attrs,
                           Args &&... args)
{
    using Applied = detail::apply_type<property_suite_builder, Exception>;
    return detail::do_builds<Applied::template type, Fixture...>(
        name, attrs, std::forward<Args>(args)...);
}

template <typename... Fixture, typename Factory, typename F>
inline runnable_suite
make_property_suite(const std::string &name, const attributes &attrs,
                    Factory &&factory, F &&f)
{
    return make_basic_property_suite<mettle::expectation_error, Fixture...>(
        name, attrs, std::forward<Factory>(factory), std::forward<F>(f));
}

template <typename... Fixture, typename Factory, typename F>
inline std::array<mettle::runnable_suite, std::max(sizeof...(Fixture), std::size_t(1))>
make_property_suites(const std::string &name, const mettle::attributes &attrs,
                     Factory &&factory, F &&f)
{
    return make_basic_property_suites<expectation_error, Fixture...>(
        name, attrs, std::forward<Factory>(factory), std::forward<F>(f));
}

template <typename Exception, typename... Fixture>
struct property_suite_base
{
    template <typename... Args>
    property_suite_base(const std::string &name, Args &&... args)
    {
        auto &&suites = make_basic_property_suites<Exception, Fixture...>(
            name, {}, std::forward<Args>(args)...);

        suites_list &list = detail::all_suites();

        for (auto &&i : suites)
            list.push_back(std::move(i));
    }
};

template <typename... T>
using property_suite = property_suite_base<expectation_error, T...>;

} // namespace mettle
