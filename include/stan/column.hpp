#pragma once

#include <stan/value.hpp>
#include <stan/pitch.hpp>
#include <stan/duration.hpp>
#include <stan/equal.hpp>

#include <variant>
#include <memory>
#include <numeric>

namespace stan {

struct rest;
struct note;
struct chord;
struct beam;
struct tuplet;
struct meter;
struct clef;

using column = std::variant<rest, note, chord, beam, tuplet, meter, clef>;

duration operator+(const duration &d, const column &c);

} // namespace stan

#include <stan/rest.hpp>
#include <stan/note.hpp>
#include <stan/chord.hpp>
#include <stan/beam.hpp>
#include <stan/tuplet.hpp>
#include <stan/meter.hpp>
#include <stan/clef.hpp>
