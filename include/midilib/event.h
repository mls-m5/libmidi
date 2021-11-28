#pragma once

#include <cstdint>
#include <iosfwd>

namespace midilib {

struct Event {
    using DeltaTimeT = uint32_t;

    Event(std::istream &file);

    Event(DeltaTimeT delta);

    DeltaTimeT delta() const {
        return _delta;
    }

private:
    DeltaTimeT _delta;
};

} // namespace midilib
