#pragma once

#include "metaevent.h"
#include "midimessage.h"
#include <cstdint>
#include <iosfwd>
#include <vector>

namespace midilib {

struct Track {
    Track(std::istream &file);

    uint32_t size;

    // Should always be first
    std::vector<MetaEvent> metaEvents;
    std::vector<MidiMessage> messages;
};

} // namespace midilib
