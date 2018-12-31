#include <stan/notation.hpp>
#include <stan/notation/copy.hpp>

#include <stan/driver/lilypond.hpp>
#include <stan/driver/debug.hpp>

// #define BOOST_SPIRIT_X3_DEBUG
#include <boost/spirit/home/x3.hpp>

#include <fstream>
#include <memory>
#include <numeric>
#include <iostream>

namespace stan {

// Stan is designed with extensive safety features that make it impossible to
// construct invalid musical objects.  Most of this is *static* safety,
// including the lack of default constructors.  Unfortunately, the lack of
// default constructors makes Spirit X3 very sad.  Instead of breaking the
// safety features for all of stan just to satisfy X3, here we will just bolt
// on default constructors in this file only.  This is still pretty safe,
// because X3 won't allow invalid values to be parsed (that is it's purpose).
// So default_ctor<T> is just a class T with a default constructor defined.
// Specializations of default_value<T> follow, which actually define what the
// default constructor should instantiate.

template <typename T>
auto default_value = T{};

template <typename T>
struct default_ctor : T
{
    default_ctor() :
        T{ stan::default_value<T> } {}

    default_ctor(const T &v) :
        T(v) {}
};

template <>
const auto default_value<stan::pitch> = stan::pitch{
    stan::pitchclass::c,
    stan::octave{ 4 }
};

template <>
const auto default_value<stan::value> = stan::value{
    stan::value::quarter()
};

template <>
const auto default_value<stan::rest> = stan::rest{
    default_value<stan::value>
};

template <>
const auto default_value<stan::note> = stan::note{
    default_value<stan::value>,
    default_value<stan::pitch>
};

template <>
const auto default_value<stan::chord> = stan::chord{
    default_value<stan::value>,
    stan::pitch{ stan::pitchclass::c, stan::octave{ 4 } },
    stan::pitch{ stan::pitchclass::e, stan::octave{ 4 } },
    stan::pitch{ stan::pitchclass::g, stan::octave{ 4 } },
};

template <>
const auto default_value<beam> = beam{
    note{ value::eighth(), default_value<pitch> },
    note{ value::eighth(), default_value<pitch> }
};

template <>
const auto default_value<tuplet> = tuplet{
    value::quarter(),
    note{ value::eighth(), default_value<pitch> },
    note{ value::eighth(), default_value<pitch> },
    note{ value::eighth(), default_value<pitch> },

};

template <>
const auto default_value<meter> = meter{
    { 4 },
    value::quarter()
};

template <>
const auto default_value<clef> = clef { 
	clef::type::treble
};

template <>
const auto default_value<key> = key { 
	pitchclass::c,
	mode::major
};

template <>
const auto default_value<stan::column> = stan::column{
    default_value<stan::note>
};

} // namespace stan

namespace boost::spirit::x3::traits {

// Spirit X3 still uses the antique boost::variant for modeling choice values,
// but the C++17 std::variant is used everywhere else in stan.  The template class
// transform_attribute<> is X3's specialization hook to convert otherwise
// incompatible attribute types, which in this case converts from
// boost::variant<> to std::variant<>.  This method works perfect for what I
// needed, but it did take a day's labor to figure this out.  If X3 ever gets
// updated to replace boost::variant<> with std::variant<> under the covers,
// this specialization may be removed.

template <typename T>
struct transform_attribute<T, stan::default_ctor<T>, x3::parser_id>
{
    using type = stan::default_ctor<T>;
    using exposed_type = T;

    static type pre(const exposed_type &ev) { return stan::default_value<T>; }

    static void post(exposed_type &ev, const type &bv)
    {
        ev = static_cast<const T &>(bv);
    }
};

} // namespace boost::spirit::x3::traits

namespace x3 = boost::spirit::x3;

namespace stan::lilypond {

struct pitchclass_ : x3::symbols<stan::pitchclass>
{
    pitchclass_()
    {
        using pc = stan::pitchclass;

        // clang-format off
        add
	    ("aff", pc::aff)("af", pc::af)("a", pc::a)("as", pc::as)("ass", pc::ass)
	    ("bff", pc::bff)("bf", pc::bf)("b", pc::b)("bs", pc::bs)("bss", pc::bss)
	    ("cff", pc::cff)("cf", pc::cf)("c", pc::c)("cs", pc::cs)("css", pc::css)
	    ("dff", pc::dff)("df", pc::df)("d", pc::d)("ds", pc::ds)("dss", pc::dss)
	    ("eff", pc::eff)("ef", pc::ef)("e", pc::e)("es", pc::es)("ess", pc::ess)
	    ("fff", pc::fff)("ff", pc::ff)("f", pc::f)("fs", pc::fs)("fss", pc::fss)
	    ("gff", pc::gff)("gf", pc::gf)("g", pc::g)("gs", pc::gs)("gss", pc::gss)
	;
        // clang-format on
    }
} pitchclass;

struct clef_ : x3::symbols<stan::clef::type>
{
    clef_()
    {
    // clang-format off
	add
	    ("treble", stan::clef::type::treble)
	    ("alto", stan::clef::type::alto)
	    ("tenor", stan::clef::type::tenor)
	    ("bass", stan::clef::type::bass)
	    ("percussion", stan::clef::type::percussion)
	    ;
    // clang-format on
    }
} clef;

struct mode_ : x3::symbols<std::vector<std::uint8_t>>
{
    mode_()
    {
    // clang-format off
	add
	    ("\\major", stan::mode::major)
	    ("\\minor", stan::mode::minor)
	    ;
    // clang-format on
    }
} mode;

struct basevalue_ : x3::symbols<default_ctor<stan::value>>
{
    basevalue_()
    {
        // clang-format off
        add
	    ("1", stan::value::whole())
	    ("2", stan::value::half())
	    ("4", stan::value::quarter())
	    ("8", stan::value::eighth())
	    ("16", stan::value::sixteenth())
	    ("32", stan::value::thirtysecond())
	    ("64", stan::value::sixtyfourth())
	    ;
        // clang-format on
    }
} basevalue;

// struct clef_ : x3::symbols<stan::clef> {
//     clef_() {
//         add
//         ("bass", stan::clef::bass())
//         ("treble", stan::clef::treble())
//         ;
//     }
// } clef_library;

// struct mode_ : x3::symbols<std::string> {
//     mode_() {
//         add
//         ("\\major", "major")
//         ("\\minor", "minor")
//         ;
//     }
// } mode;

using boost::fusion::at_c;
using x3::_attr;
using x3::_val;
using x3::attr;
using x3::eps;
using x3::lit;
using x3::repeat;
using x3::string;
using x3::ushort_;
using x3::ascii::char_;

x3::rule<struct ppitch, default_ctor<stan::pitch>> ppitch = "pitch";
x3::rule<struct poctave, stan::octave> poctave = "octave";
x3::rule<struct pvalue, default_ctor<stan::value>> pvalue = "value";
x3::rule<struct prest, default_ctor<stan::rest>> prest = "rest";
x3::rule<struct pnote, default_ctor<stan::note>> pnote = "note";
x3::rule<struct pchord, default_ctor<stan::chord>> pchord = "chord";
x3::rule<struct pbeam, default_ctor<stan::beam>> pbeam = "beam";
x3::rule<struct ptuplet, default_ctor<stan::tuplet>> ptuplet = "tuplet";
x3::rule<struct pmeter, default_ctor<stan::meter>> pmeter = "meter";
x3::rule<struct pclef, default_ctor<stan::clef>> pclef = "clef";
x3::rule<struct pkey, default_ctor<stan::key>> pkey = "key";
x3::rule<struct pcolumn, default_ctor<stan::column>> column = "column";

// x3::rule<struct pmusic, std::shared_ptr<stan::column>> music = "music";
// x3::rule<struct music_list, stan::sequential> music_list = "music_list";
// x3::rule<struct key, stan::key> key = "key";
// x3::rule<struct clef, stan::clef> clef = "clef";

// Define semantic actions separately, because C++ reserves [[]] syntax.  The
// semantic actions count the number of octave ticks, and convert them to a
// small number that is actually stored in stan::octave.
auto raise_octave = [](auto &ctx) {
    _val(ctx) = stan::octave{ static_cast<std::uint8_t>(4 + _attr(ctx).size()) };
};
auto lower_octave = [](auto &ctx) {
    _val(ctx) = stan::octave{ static_cast<std::uint8_t>(4 - _attr(ctx).size()) };
};
auto default_octave = [](auto &ctx) { _val(ctx) = stan::octave{ 4 }; };
auto const poctave_def =
    repeat(1, /*(int)stan::octave::max()*/ 7 - 4)[char_(R"(')")][raise_octave] |
    repeat(1, 4 - /*(int)stan::octave::min()*/ 0)[char_(R"(,)")][lower_octave] |
    eps[default_octave];

// Use semantic actions to maintain a running value, for parses like "{ c4 d }".
struct value_tag
{
};
//auto store_running_value = [](auto& ctx) { x3::get<value_tag>(ctx).get() = _attr(ctx); };
// auto use_running_value = [](auto& ctx) { _attr(ctx) = x3::get<value_tag>(ctx); };
// auto const note_def = note %= pitch >>
//    (value/*[store_running_value]*/ | attr(stan::value::quarter())/*[use_running_value]*/);

template <typename T, int... ArgOrder>
struct construct
{
    template <typename Context>
    void operator()(Context &ctx)
    {
        x3::_val(ctx) = T{ at_c<ArgOrder>(x3::_attr(ctx))... };
    }
};

template <typename T>
struct construct<T>
{
    template <typename Context>
   void operator()(Context &ctx)
    {
        x3::_val(ctx) = T{ x3::_attr(ctx) };
    }
};

// Metaprogram to compute a boost::variant<> from std::variant<>.
template <typename T>
struct std_variant_to_boost;

template <typename... Ts>
struct std_variant_to_boost<std::variant<Ts...>>
{
    using type = boost::variant<Ts...>;
};

template <>
struct construct<stan::column>
{
    template <typename Context>
    void operator()(Context &ctx)
    {
        typename std_variant_to_boost<stan::column>::type v = x3::_attr(ctx);
        x3::_val(ctx) = stan::column(stan::copy(v));
    }
};

auto to_tuplet = [](auto &ctx) {
    auto attr = _attr(ctx);
    stan::value val = tuplet::scale(at_c<0>(attr), at_c<1>(attr), at_c<2>(attr));
    x3::_val(ctx) = tuplet{ val, at_c<2>(attr) };
};

auto to_meter = [](auto &ctx) {
    auto attr = _attr(ctx);
    // This works only for simple meter so far
    x3::_val(ctx) = meter{
        { static_cast<std::uint8_t>(at_c<0>(attr)) },
        at_c<1>(attr)
    };
};

auto const prest_def = x3::lit('r') >> pvalue[construct<stan::rest>()];
auto const pnote_def = (ppitch >> pvalue)[construct<stan::note, 1, 0>()];
auto const ppitch_def = (pitchclass >> poctave)[construct<stan::pitch, 0, 1>()];

auto add_dot = [](auto &ctx) { _val(ctx) = dot(_val(ctx)); };

auto const pvalue_def =
    basevalue[construct<stan::value>()] >> x3::repeat(0, 2)[lit('.')[add_dot]];
auto const pchord_def = ('<' >> +ppitch >> '>' >> pvalue)[construct<stan::chord, 1, 0>()];
auto const pbeam_def = '[' >> (+column)[construct<stan::beam>()] >> ']';
auto const ptuplet_def =
    (lit(R"(\tuplet)") >> x3::int_ >> '/' >> x3::int_ >> '{' >> (+column) >> '}')
        [to_tuplet];
auto const pmeter_def =
    (lit(R"(\time)") >> x3::ushort_ >> '/' >> basevalue)[to_meter];
auto const pclef_def =
    (lit(R"(\clef)") >> clef)[construct<stan::clef>()];
auto const pkey_def = 
    (lit(R"(\key)") >> pitchclass >> mode)[construct<stan::key, 0, 1>()];
auto const column_def = (prest | pnote | pchord | pbeam | ptuplet | pmeter | pclef | pkey)
    [construct<stan::column>()];
// auto make_shared = [](auto &ctx) { _val = std::make_shared<column>(std::move(_attr(ctx))); };
// auto const music_def = column[make_shared];
// auto const variant_def = note | chord_body | key | meter | clef ;
// auto const music_list_def = lit('{') >> +variant >> '}';

BOOST_SPIRIT_DEFINE(ppitch)
BOOST_SPIRIT_DEFINE(poctave)
BOOST_SPIRIT_DEFINE(pvalue)
BOOST_SPIRIT_DEFINE(prest)
BOOST_SPIRIT_DEFINE(pnote)
BOOST_SPIRIT_DEFINE(pchord)
BOOST_SPIRIT_DEFINE(pbeam)
BOOST_SPIRIT_DEFINE(ptuplet)
BOOST_SPIRIT_DEFINE(pmeter)
BOOST_SPIRIT_DEFINE(pclef)
BOOST_SPIRIT_DEFINE(pkey)
BOOST_SPIRIT_DEFINE(column)

stan::column reader::operator()(const std::string &lily)
{
    stan::column music{ stan::default_value<stan::note> };
    auto iter = lily.begin();

    if (!x3::phrase_parse(iter, lily.end(), column, x3::space, music)) {
        throw std::runtime_error("parse error");
    }

    if (iter != lily.end()) {
        throw std::runtime_error("incomplete parse");
    }

    return std::move(music);
}

} // namespace stan::lilypond
