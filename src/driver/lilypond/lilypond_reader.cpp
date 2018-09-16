#include <stan/driver/lilypond.hpp>

// #define BOOST_SPIRIT_X3_DEBUG
#include <boost/spirit/home/x3.hpp>

#include <boost/hana/find_if.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/not_equal.hpp>

#include <fstream>
#include <memory>

namespace stan::lilypond {

struct value : stan::value
{
    value() : stan::value(stan::value::quarter()) {}  
    value(const stan::value& v) : stan::value(v) {}
};

struct pitch : stan::pitch
{
    pitch() : stan::pitch(stan::pitchclass::c, stan::octave{4}) {}  
    pitch(const stan::pitch& v) : stan::pitch(v) {}
};

struct note : stan::note
{
    note() : stan::note(stan::value::quarter(),  { stan::pitchclass::c, stan::octave{4} }) {}  
    note(const stan::note& v) : stan::note(v) {}
};

// struct column : stan::column
// {
//     column() = default;
// };

} // stan::lilypond

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

    static type pre(const exposed_type& ev) { return std::move(type()); }

    static void post(exposed_type& ev, const type& bv) {
        ev = boost::apply_visitor([](auto&& n) -> exposed_type { return std::move(n); }, bv);
    }
};

template <>
void move_to(stan::lilypond::note&& src, stan::column& dest)
{
	dest.m_variant = src;
}

} // boost::spirit::x3::traits

// Metaprogram to compute a boost::variant<> from std::variant<>.
template <typename T> struct std_variant_to_boost;

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
BOOST_FUSION_ADAPT_STRUCT(stan::pitch, m_pitchclass, m_octave);
// BOOST_FUSION_ADAPT_STRUCT(stan::value, base, dots);
BOOST_FUSION_ADAPT_STRUCT(stan::note, m_value, m_pitch);
// BOOST_FUSION_ADAPT_STRUCT(stan::chord, m_value, m_pitches);
// BOOST_FUSION_ADAPT_STRUCT(stan::key, pitchclass, mode);
// BOOST_FUSION_ADAPT_STRUCT(stan::meter, beats, value);

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;
namespace hana = boost::hana;

namespace stan::lilypond {

struct pitchclass_ : x3::symbols<stan::pitchclass> {
    pitchclass_() {
        using pc = stan::pitchclass;

        add
            ("aff", pc::aff)("af", pc::af)("a", pc::a)("as", pc::as)("ass", pc::ass)
            ("bff", pc::bff)("bf", pc::bf)("b", pc::b)("bs", pc::bs)("bss", pc::bss)
            ("cff", pc::cff)("cf", pc::cf)("c", pc::c)("cs", pc::cs)("css", pc::css)
            ("dff", pc::dff)("df", pc::df)("d", pc::d)("ds", pc::ds)("dss", pc::dss)
            ("eff", pc::eff)("ef", pc::ef)("e", pc::e)("es", pc::es)("ess", pc::ess)
            ("fff", pc::fff)("ff", pc::ff)("f", pc::f)("fs", pc::fs)("fss", pc::fss)
            ("gff", pc::gff)("gf", pc::gf)("g", pc::g)("gs", pc::gs)("gss", pc::gss)
            ;
    }
} pitchclass;

struct basevalue_ : x3::symbols<value> {
    basevalue_() {
        add
            ("1", value::whole())
            ("2", value::half())
            ("4", value::quarter())
            ("8", value::eighth())
            ("16", value::sixteenth())
            ("32", value::thirtysecond())
            ("64", value::sixtyfourth())
            ;
    }
} basevalue;

struct dovalue_ : x3::symbols<stan::value::dots_t> {
    dovalue_() {
        add
            (".", 1)
            ("..", 2)
            ;
    };
} dovalue;

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
using x3::ushort_;
using x3::_val;
using x3::_attr;
using x3::repeat;
using x3::eps;
using x3::attr;
using x3::lit;
using x3::string;

x3::rule<struct ppitch, pitch> ppitch = "pitch";
x3::rule<struct poctave, stan::octave> poctave = "octave";
x3::rule<struct pvalue, value> pvalue = "value";
x3::rule<struct pnote, note> pnote = "note";
// x3::rule<struct chord_body, stan::chord> chord_body = "chord_body";
x3::rule<struct pcolumn> column = "column";
// x3::rule<struct variant, std::variant_to_boost<stan::variant>::type> variant = "variant";
// x3::rule<struct music_list, stan::sequential> music_list = "music_list";
// x3::rule<struct key, stan::key> key = "key";
// x3::rule<struct meter, stan::meter> meter = "meter";
// x3::rule<struct clef, stan::clef> clef = "clef";

// Define semantic actions separately, because C++ reserves [[]] syntax.  The
// semantic actions count the number of octave ticks, and convert them to a
// small number that is actually stored in stan::octave.
auto raise_octave = [](auto& ctx) { _val(ctx) = stan::octave{ 4 + _attr(ctx).size() }; };
auto lower_octave = [](auto& ctx) { _val(ctx) = stan::octave{ 4 - _attr(ctx).size() }; };
auto default_octave = [](auto& ctx) { _val(ctx) = stan::octave{ 4 }; };
auto const poctave_def =
    repeat(1,/*(int)stan::octave::max()*/7-4)[char_(R"(')")][raise_octave] |
    repeat(1,4-/*(int)stan::octave::min()*/0)[char_(R"(,)")][lower_octave] |
    eps[default_octave];

// Use semantic actions to maintain a running value, for parses like "{ c4 d }".
struct value_tag {};
//auto store_running_value = [](auto& ctx) { x3::get<value_tag>(ctx).get() = _attr(ctx); };
// auto use_running_value = [](auto& ctx) { _attr(ctx) = x3::get<value_tag>(ctx); };
// auto const note_def = note %= pitch >>
//    (value/*[store_running_value]*/ | attr(stan::value::quarter())/*[use_running_value]*/);

auto to_pitch = [](auto& ctx) { return pitch(); };
auto to_value = [](auto& ctx) { return value(); };
auto to_note = [](auto& ctx) { return stan::note { stan::value::quarter(), stan::pitch { pitchclass::c, stan::octave{4} } }; } ;
auto const pnote_def = (ppitch >> pvalue)[to_note];
auto const ppitch_def = (pitchclass >> poctave)[to_pitch];
auto const pvalue_def = (basevalue >> (dovalue | attr(stan::value::dots_t(0))))[to_value];
// auto const chord_body_def = (lit('<') >> +ppitch >> '>' >> pvalue);

//auto unique = [](auto& ctx) { /* _val(ctx) = stan::column::adapt(_attr(ctx)); */ };
auto const column_def = pnote;//(note)[unique];
// auto const variant_def = note | chord_body | key | meter | clef ;
// auto const music_list_def = lit('{') >> +variant >> '}';

// BOOST_SPIRIT_DEFINE(pitch, octave, value, note, column) // , chord_body, variant, music_list);

BOOST_SPIRIT_DEFINE(ppitch)
BOOST_SPIRIT_DEFINE(poctave)
BOOST_SPIRIT_DEFINE(pvalue)
BOOST_SPIRIT_DEFINE(pnote)
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
stan::column parse(const std::string& lily)
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
    stan::pitch p { pitchclass::c, stan::octave{4} } ;
    // value ev; 
    // stan::note ev { stan::value::quarter(), p };
    struct column ev { stan::note { stan::value::quarter(), p } };
    auto iter = lily.begin();
    if (!x3::phrase_parse(iter, lily.end(), column, x3::space, ev))
    {
        throw std::runtime_error("parse error");
    }
    if (iter != lily.end())
    {
        throw std::runtime_error("incomplete parse");
    }

    // stan::column& e = ev;
    // return e;
    // static_cast<stan::column>(ev);
    return ev;
}

// template <typename Event>
stan::column parse(std::ifstream& is)
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

stan::column reader::operator()(const std::string& lily)
{
    return parse(lily);
}


} // stan::lilypond

