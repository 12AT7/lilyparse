#include <stan/column.hpp>
#include <stan/driver/lilypond.hpp>
#include <stan/driver/debug.hpp>
#include "to_printable.hpp"

#include <mettle.hpp>
#include <stan/rapidcheck/mettle.hpp>

using mettle::equal_to;
using mettle::expect;
using mettle::regex_match;

mettle::property_suite<> lilypond_suite("lilypond reader", [](auto &_) {
    using pc = stan::pitchclass;
    using stan::value;
    using stan::octave;
    using stan::pitch;
    using stan::column;

    static stan::note c4{ value::quarter(), pitch{ pc::c, octave{ 4 } } };
    static stan::lilypond::reader read;
    static stan::lilypond::writer write;

    _.property("note", [](stan::note n) {
        std::string lily = write(n);
        std::cout << lily << std::endl;
        column c = read(lily);
        std::cout << "column: " << write(c) << std::endl;
        expect(read(lily), equal_to(n));
    });
});
