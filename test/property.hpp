#include <stan/rapidcheck/mettle.hpp>

template <typename Builder, typename Testable>
auto property(Builder &_, std::string name, Testable testable)
{
    _.test(name, [testable]() {
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
    });
}
