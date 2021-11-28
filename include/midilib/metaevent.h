#pragma once

#include "event.h"
#include <cstdint>
#include <iosfwd>
#include <string_view>
#include <vector>

namespace midilib {

struct MetaEvent : public Event {
    MetaEvent(std::istream &file, DeltaTimeT time);

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

        SequencerSpecificEvent = 0x7f,

        Error = 0xffff,
        NotImplemented = 0xffff - 1,
    };

    // Get primary event type
    Type type() const;

    // For sequence number event only
    uint16_t sequenceNumber() const;

    // Text for text events
    std::string_view text() const;

    // Get size of data
    size_t size() const;

    // Get the raw data of the event
    const char *data() const;

    void print(std::ostream &stream);

    std::string_view name();

private:
    Type _type = Error;
    std::vector<char> _data;
};

} // namespace midilib
