#pragma once

#include <stan/notation/duration.hpp>

#include <variant>

namespace stan {

struct rest;
struct note;
struct chord;
struct beam;
struct tuplet;
struct meter;
struct clef;
struct key;

using column = std::variant<rest, note, chord, beam, tuplet, meter, clef, key>;

duration operator+(const duration &d, const column &c);

} // namespace stan

