#pragma once

#include "event.h"
#include <cstdint>
#include <iosfwd>
#include <string_view>
#include <vector>

namespace midilib {

struct MetaEvent : public Event {
    MetaEvent(std::istream &file);

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

        EndOfTrack = 0x2f,

        Error = 0xffff,
        NotImplemented = 0xffff - 1,
    };

    Type type() const;

    // Get size of data
    size_t size() const;

    // Get the raw data of the event
    const char *data() const;

    // For sequence number event
    uint16_t sequenceNumber() const;

    std::string_view text() const;

private:
    Type _type = Error;
    std::vector<char> _data;
};

} // namespace midilib
