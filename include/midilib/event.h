#pragma once

#include <cstdint>
#include <iosfwd>

namespace midilib {

struct Event {
    Event(std::istream &file);

    uint32_t delta() const {
        return _delta;
    }

private:
    uint32_t _delta;
};

} // namespace midilib
