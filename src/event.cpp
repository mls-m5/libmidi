#include "midilib/event.h"
#include "bitops.h"

namespace midilib {

Event::Event(std::istream &file)
    : _delta{readVarInt(file)} {}

Event::Event(DeltaTimeT delta)
    : _delta{delta} {}

} // namespace midilib
