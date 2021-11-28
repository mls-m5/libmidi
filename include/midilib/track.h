#pragma once

#include "metaevent.h"
#include "midimessage.h"
#include <cstdint>
#include <iosfwd>
#include <variant>
#include <vector>

namespace midilib {

struct Track {
    Track(std::istream &file);

    uint32_t size;

    // Should always be first
    std::vector<std::variant<MidiMessage, MetaEvent>> events;

    // Pretty print to screen
    void print(std::ostream &stream);
};

} // namespace midilib
