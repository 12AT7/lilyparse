#include <stan/driver/lilypond.hpp>
#include <stan/driver/debug.hpp>

// #define BOOST_SPIRIT_X3_DEBUG
#include <boost/spirit/home/x3.hpp>

#include <boost/hana/find_if.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/not_equal.hpp>

#include <fstream>
#include <memory>

namespace stan::lilypond {

// struct value : stan::value
// {
//     value() :
//         stan::value(stan::value::whole()) {}
//     value(const stan::value &v) :
//         stan::value(v) {}
// };

// Stan is designed with extensive safety features that make it impossible to
// construct invalid musical objects.  Most of this is static safety, including
// the lack of default constructors.  Lacking default constructors for the data
// model, Spirit X3 becomes very sad.  Instead of breaking the safety features
// for all of stan, here we will just bolt on default constructors in this file
// only.  This is still pretty safe, because X3 won't allow invalid values to
// be parsed (that is it's purpose).

template <typename T>
auto default_value = T{};

template <>
static const auto default_value<stan::pitch> = stan::pitch{ stan::pitchclass::c, stan::octave{ 4 } };

template <>
static const auto default_value<stan::value> = stan::value::quarter();

template <>
static const auto default_value<stan::rest> = stan::rest{ default_value<stan::value> };

template <>
static const auto default_value<stan::note> = stan::note{ default_value<stan::value>, default_value<stan::pitch> };

template <>
static const auto default_value<stan::chord> = stan::chord{ default_value<stan::value>, std::vector<stan::pitch>{ default_value<stan::pitch> } };

template <typename T>
struct default_ctor : T
{
    default_ctor() :
        T{ default_value<T> } {}
    default_ctor(const T &v) :
        T{ v } {}
};

// struct pitch : stan::pitch
// {
//     pitch() :
//         stan::pitch(stan::pitchclass::d, stan::octave{ 5 }) {}
//     pitch(const stan::pitch &v) :
//         stan::pitch(v) {}
// };

// struct note : stan::note
// {
//     note() :
//         stan::note(stan::value::half(), { stan::pitchclass::e, stan::octave{ 3 } }) {}
//     note(stan::note &&v) :
//         stan::note(v) {}
// };

// struct chord : stan::chord
// {
//     chord() :
//         stan::chord(stan::value::half(), std::vector<stan::pitch>{ { stan::pitchclass::c, stan::octave{ 4 } } }) {}
//     chord(stan::chord &&v) :
//         stan::chord(v) {}
// };

// struct rest : stan::rest
// {
//     rest() :
//         stan::rest(stan::value::half()) {}
//     rest(stan::rest &&v) :
//         stan::rest(v) {}
// };

// struct column : stan::column
// {
//     column() = default;
// };

} // namespace stan::lilypond

namespace boost::spirit::x3::traits {

// Spirit X3 still uses the antique boost::variant for modeling choice values,
// but the C++17 std::variant is used everywhere else in stan.  The template class
// transform_attribute<> is X3's specialization hook to convert otherwise
// incompatible attribute types, which in this case converts from
// boost::variant<> to std::variant<>.  This method works perfect for what I
// needed, but it did take a day's labor to figure this out.  If X3 ever gets
// updated to replace boost::variant<> with std::variant<> under the covers,
// this specialization may be removed.

template <typename... Ts>
struct transform_attribute<std::variant<Ts...>, boost::variant<Ts...>, x3::parser_id>
{
    using type = boost::variant<Ts...>;
    using exposed_type = std::variant<Ts...>;

    static type pre(const exposed_type &ev) { return std::move(type()); }

    static void post(exposed_type &ev, const type &bv)
    {
        ev = boost::apply_visitor([](auto &&n) -> exposed_type { return std::move(n); }, bv);
    }
};

} // namespace boost::spirit::x3::traits

// Metaprogram to compute a boost::variant<> from std::variant<>.
template <typename T>
struct std_variant_to_boost;

template <typename... Ts>
struct std_variant_to_boost<std::variant<Ts...>>
{
    using type = boost::variant<Ts...>;
};

// Spirit uses boost::fusion for struct introspection, while the rest of
// stan uses boost::hana.  The best way out of this disagreement is for
// X3 to someday support hana natively.  In the meantime, there is probably
// a way to metaprogram the hana->fusion structures automatically, but the
// expedient way forward is just these set of macros that (sadly) must be
// manually maintained.
// BOOST_FUSION_ADAPT_STRUCT(stan::pitch, m_pitchclass, m_octave);
// BOOST_FUSION_ADAPT_STRUCT(stan::value, base, dots);
// BOOST_FUSION_ADAPT_STRUCT(stan::rest, m_value);
// BOOST_FUSION_ADAPT_STRUCT(stan::note, m_value, m_pitch);
// BOOST_FUSION_ADAPT_STRUCT(stan::chord, m_value, m_pitches);
// BOOST_FUSION_ADAPT_STRUCT(stan::key, pitchclass, mode);
// BOOST_FUSION_ADAPT_STRUCT(stan::meter, beats, value);

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;
namespace hana = boost::hana;

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

struct basevalue_ : x3::symbols<default_ctor<stan::value>>
{
    basevalue_()
    {
        // clang-format off
        add
	    ("1", value::whole())
	    ("2", value::half())
	    ("4", value::quarter())
	    ("8", value::eighth())
	    ("16", value::sixteenth())
	    ("32", value::thirtysecond())
	    ("64", value::sixtyfourth())
	    ;
        // clang-format on
    }
} basevalue;

// struct dotvalue_ : x3::symbols<int>
// {
//     dotvalue_()
//     {
//         add(".", int{ 1 })("..", int{ 2 });
//     };
// } dotvalue;

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

using ascii::char_;
using boost::fusion::at_c;
using x3::_attr;
using x3::_val;
using x3::attr;
using x3::eps;
using x3::lit;
using x3::repeat;
using x3::string;
using x3::ushort_;

x3::rule<struct ppitch, default_ctor<pitch>> ppitch = "pitch";
x3::rule<struct poctave, stan::octave> poctave = "octave";
x3::rule<struct pvalue, default_ctor<stan::value>> pvalue = "value";
x3::rule<struct prest, default_ctor<rest>> prest = "rest";
x3::rule<struct pnote, default_ctor<stan::note>> pnote = "note";
x3::rule<struct pchord, default_ctor<chord>> pchord = "chord";
using variant2 = boost::variant<stan::rest, stan::note, stan::chord>;
x3::rule<struct pcolumn, variant2> column = "column";
// x3::rule<struct variant, std::variant_to_boost<stan::variant>::type> variant = "variant";
// x3::rule<struct music_list, stan::sequential> music_list = "music_list";
// x3::rule<struct key, stan::key> key = "key";
// x3::rule<struct meter, stan::meter> meter = "meter";
// x3::rule<struct clef, stan::clef> clef = "clef";

// Define semantic actions separately, because C++ reserves [[]] syntax.  The
// semantic actions count the number of octave ticks, and convert them to a
// small number that is actually stored in stan::octave.
auto raise_octave = [](auto &ctx) { _val(ctx) = stan::octave{ 4 + _attr(ctx).size() }; };
auto lower_octave = [](auto &ctx) { _val(ctx) = stan::octave{ 4 - _attr(ctx).size() }; };
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

// auto to_pitch = [](auto &ctx) {
//     _val(ctx) = stan::pitch{ at_c<0>(_attr(ctx)), at_c<1>(_attr(ctx)) };
// };
// auto to_value = [](auto &ctx) {
//     _val(ctx) = lilypond::value(at_c<0>(_attr(ctx)));
//     int dots = boost::get<int>(at_c<1>(_attr(ctx)));
//     for (auto i = 0; i < dots; ++i) {
//         _val(ctx) = dot(_val(ctx));
//     }
// };

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

// auto to_rest = [](auto &ctx) {
//     _val(ctx) = stan::rest{ _attr(ctx) };
// };
// auto to_note = [](auto &ctx) {
//     _val(ctx) = stan::note{ at_c<1>(_attr(ctx)), at_c<0>(_attr(ctx)) };
// };
// auto to_chord = [](auto &ctx) {
//     std::vector<stan::pitch> pitches;
//     auto p = at_c<0>(_attr(ctx));
//     std::copy(p.begin(), p.end(), std::back_inserter(pitches));
//     _val(ctx) = stan::chord{ at_c<1>(_attr(ctx)), pitches };
// };
auto const prest_def = x3::lit('r') >> pvalue[construct<rest>()];
auto const pnote_def = (ppitch >> pvalue)[construct<stan::note, 1, 0>()];
auto const ppitch_def = (pitchclass >> poctave)[construct<stan::pitch, 0, 1>()];
// auto const pvalue_def = (basevalue >> (dotvalue | attr(int(0))))[construct<stan::value, 0>()];

auto add_dot = [](auto &ctx) { _val(ctx) = dot(_val(ctx)); };

auto const pvalue_def = basevalue[construct<stan::value>()] >> x3::repeat(0, 2)[lit('.')[add_dot]];
auto const pchord_def = ('<' >> +ppitch >> '>' >> pvalue)[construct<stan::chord, 1, 0>()];

auto const column_def = prest | pnote | pchord;
// auto const variant_def = note | chord_body | key | meter | clef ;
// auto const music_list_def = lit('{') >> +variant >> '}';

// BOOST_SPIRIT_DEFINE(pitch, octave, value, note, column) // , chord_body, variant, music_list);

BOOST_SPIRIT_DEFINE(ppitch)
BOOST_SPIRIT_DEFINE(poctave)
BOOST_SPIRIT_DEFINE(pvalue)
BOOST_SPIRIT_DEFINE(prest)
BOOST_SPIRIT_DEFINE(pnote)
BOOST_SPIRIT_DEFINE(pchord)
BOOST_SPIRIT_DEFINE(column)

// auto construct_key = [](auto& ctx) {
//     // _attr(ctx) = stan::key(stan::pitchclass::d, "minor");
//     _attr(ctx) = stan::key(boost::fusion::at_c<0>(_val(ctx)), boost::fusion::at_c<1>(_val(ctx)));
// };
// auto const key_def = key %= lit("\\key") >> (pitchclass >> mode)[construct_key];
// auto const meter_def = lit("\\time") >> ushort_ >> '/' >> basevalue;
// auto const clef_def = lit("\\clef") >> clef_library;
//
// BOOST_SPIRIT_DEFINE(key, meter, clef);

// Keep a master list of all defined rules.  This helps Hana metaprograms
// automatically associate requested attribute types with rules, and explicit
// template instantiations, reducing boilerplate.
//auto rules  = hana::make_tuple(
//       column
// pitch, octave, value, note
// chord_body, variant, music_list,
// key, meter, clef
// );

// template <typename Event>
stan::column
parse(const std::string &lily)
{
// Define a boolean predicate to detect if a parsing rule, such as
// parse::voice, returns at attribute of type Event (true) or something
// else (false).
#if 0
    auto has_Event_attribute = [](auto rule) {
        using Attr = typename decltype(rule)::attribute_type;
        return hana::equal(hana::type_c<Attr>, hana::type_c<Event>);
    };

    // Find the specific rule that will return an Event
    auto rule = hana::find_if(rules, has_Event_attribute);
    static_assert(rule != hana::nothing, "no parse rule defined for Event");

    stan::value run = stan::value::quarter();
    auto parse = x3::with<value_tag>(std::ref(run)) [rule.value()];
#endif
    // Now that the appropriate rule is discovered, use it to parse an Event
    // stan::pitch p{ pitchclass::g, stan::octave{ 2 } };
    // struct column ev
    // {
    //     stan::note { stan::value::quarter(), p }
    // };
    variant2 ev = stan::rest(stan::value::quarter());
    auto iter = lily.begin();
    if (!x3::phrase_parse(iter, lily.end(), column, x3::space, ev)) {
        throw std::runtime_error("parse error");
    }
    if (iter != lily.end()) {
        throw std::runtime_error("incomplete parse");
    }

    // std::cout << "parsed " << driver::debug::write(ev) << std::endl;
    // return stan::column(std::move(ev));
    return stan::column(boost::apply_visitor([](auto &&n) -> stan::variant { return std::move(n); }, ev));
    // return boost::apply_visitorstan::column(
}

// template <typename Event>
stan::column parse(std::ifstream &is)
{
    std::string lily{
        (std::istreambuf_iterator<char>(is)),
        (std::istreambuf_iterator<char>())
    };
    return parse(lily);
}

#if 0
// Coax explicit instantiations of specialized read<> templates for every rule
static auto parse_instances = hana::transform(rules, [](auto rule) {
    using Event = typename decltype(rule)::attribute_type;
    // return static_cast<std::unique_ptr<Event> (*)(std::ifstream&)>(std::move(parse<Event>));
    return [](std::ifstream& is) -> Event {
        auto ev = parse<Event>(is);
        return ev;
        };
});
#endif

#if 0
template<>
stan::column parse<stan::column>(std::ifstream&);
#endif

stan::column reader::operator()(const std::string &lily)
{
    return parse(lily);
}

} // namespace stan::lilypond
