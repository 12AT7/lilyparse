#include <stan/column.hpp>
#include <stan/driver/lilypond.hpp>
#include <stan/driver/debug.hpp>
#include "to_printable.hpp"

#include <mettle.hpp>
#include <stan/rapidcheck/mettle.hpp>

using mettle::equal_to;
using mettle::expect;
using mettle::regex_match;

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

mettle::suite<
    stan::rest,
    stan::note,
    stan::chord>
    column_suite(
        "lilypond reader", mettle::type_only, [](auto &_) {
            static stan::lilypond::reader read;
            static stan::lilypond::writer write;

            using Event = mettle::fixture_type_t<decltype(_)>;

            property(_, "writeread", [](Event n) {
                std::string lily = write(n);
                std::cout << "lily: " << lily << std::endl;
                expect(read(lily), equal_to(n));
            });
        });
