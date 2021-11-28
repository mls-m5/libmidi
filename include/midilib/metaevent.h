#pragma once

#include <cstdint>
#include <iosfwd>
#include <vector>

namespace midilib {

struct MetaEvent {
    // FF
    enum Type {
        SequenceNumber = 0x00,

        // Text meta avents
        TextEvent = 0x01,
        CopyrightNotice = 0x02,
        TrackName = 0x03,
        InstrumentName = 0x04,
        Lyric = 0x05,
        Marker = 0x06,
        CuePoint = 0x07,

        EndOfTrack = 0x2F,
    };

    Type type() {
        return _type;
    }

    size_t size() {
        return _data.size();
    }

    char *data() {
        return _data.data();
    }

    Type _type;
    std::vector<char> _data;

    MetaEvent(std::istream &file);

    // For sequence number event
    uint16_t sequenceNumber();
};

} // namespace midilib
