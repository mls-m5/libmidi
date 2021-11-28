#include "midilib/metaevent.h"
#include "bitops.h"
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace midilib {

void expect(char value, char expected, std::string_view message) {
    if (value != expected) {
        throw std::runtime_error{std::string{message}};
    }
}

MetaEvent::MetaEvent(std::istream &file, DeltaTimeT time)
    : Event{time} {
    {
        unsigned char c;
        file >> c;
        _type = Error;
        if (c != 0xff) {
            throw std::runtime_error{"not a meta event"};
        }
    }

    _type = readInt<Type, unsigned char>(file);

    char c;

    switch (_type) {
    case SequenceNumber:
        readInt(file, &c);
        expect(c, 0x02, "Malformed sequence number");

        _data.resize(2);
        file.read(_data.data(), 2);

        break;
    case TextEvent:
    case CopyrightNotice:
    case TrackName:
    case InstrumentName:
    case Lyric:
    case Marker:
    case CuePoint:
    case SequencerSpecificEvent: {
        auto len = readVarInt(file);
        _data.resize(len);
        file.read(_data.data(), _data.size());

    } break;

    case EndOfTrack:
        readInt(file, &c);
        expect(c, 0, "Bad formatted End of Track event");
        break;

    default:
        throw std::runtime_error{"meta event " + std::to_string(_type) +
                                 " not implemented"};
    }
}

MetaEvent::Type MetaEvent::type() const {
    return _type;
}

size_t MetaEvent::size() const {
    return _data.size();
}

const char *MetaEvent::data() const {
    return _data.data();
}

void MetaEvent::print(std::ostream &stream) {
    stream << "meta event " << std::hex << type() << std::dec << "\n";
}

uint16_t midilib::MetaEvent::sequenceNumber() const {
    expect(type(),
           SequenceNumber,
           "trying to get sequence number of wrong type of event");
    expect(_data.size(), 2, "wrong size of sequence number data");

    return readInt<uint16_t>(_data.data());
}

std::string_view MetaEvent::text() const {
    return {_data.data(), _data.size()};
}

} // namespace midilib
